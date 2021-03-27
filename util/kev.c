/*
    Copyright 2020 Alexander Vollschwitz <xelalex@gmx.net>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>

// for window focus
#include <locale.h>
#include <X11/Xlib.h>        // apt-get install libx11-dev
#include <X11/Xmu/WinUtil.h> // apt-get install libxmu-dev

// for keyboard image window
#include <gtk/gtk.h>         // apt-get install libgtk-3-dev

// logging
#include "log.h"

//
#define IMAGE_WINDOW_NAME "Spectratur"
#define WIN_NAME_BUF_SIZE 500

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

/*
    serial port code based on:
        https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c

    window focus code based on:
        https://gist.github.com/kui/2622504

    log library:
        https://github.com/rxi/log.c

 */

// constants
static const char *const keyActionTypes[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

static const int BREAK = 0;
static const int MAKE = 1;

void cleanup();

// file descriptors
int fdSerialPort = -1;
int fdKeyboard = -1;

// --- serial communication ---------------------------------------------------

//
int configure_port(int fd, int speed, int parity) {

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        log_error("error %d from tcgetattr", errno);
        return 0;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);    // ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);  // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        log_error("error %d from tcsetattr", errno);
        return 0;
    }

    return 1;
}

//
void set_blocking(int fd, int should_block) {

    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        log_error("error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        log_error("error %d setting term attributes", errno);
    }
}

//
int open_serial_port_or_die(char* port) {

    if (port == NULL) {
        log_fatal("you need to specify the serial port to use");
        exit(EXIT_FAILURE);
    }

    log_info("opening serial port %s", port);
    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        log_fatal("cannot open %s: %s", port, strerror(errno));
        exit(EXIT_FAILURE);
    }

    configure_port(fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 0); // set no blocking
    return fd;
}

//
void close_serial_port(int fd) {
    if (fd) {
        log_info("closing serial port");
        close(fd);
    }
}

//
void send_key_stroke(int typ, int code, int fdSer) {

    if (typ < 0 || typ >= LEN(keyActionTypes)) {
        return;
    }

    log_debug("%s 0x%04x (%d)", keyActionTypes[typ], code, code);

    if (typ != MAKE && typ != BREAK) {
        return;
    }

    char sendBuf[2];
    sendBuf[0] = (char)typ;
    sendBuf[1] = (char)code;

    log_debug("sending to serial: [0x%x, 0x%x]", sendBuf[0], sendBuf[1]);
    write(fdSer, &sendBuf, 2);
}

// --- keyboard image window --------------------------------------------------

//
void window_destroy(void) {
    gtk_main_quit();
    cleanup();
    log_info("exiting");
    exit(EXIT_SUCCESS);
}

// for filtering out auto repeat keys
guint16 lastPressedKey = 0;
Bool lastWasPressed = False;

//
// TODO: not sure why GDK scan codes are eight higher than kernel input event
//       codes
// see:
//      https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/input-event-codes.h#n65
//
gboolean key_pressed(GtkWidget* widget, GdkEventKey* evt, gpointer data) {
    guint16 code = evt->hardware_keycode - 8;
    if (!lastWasPressed || lastPressedKey != code) {
        send_key_stroke(1, code, fdSerialPort);
        lastPressedKey = code;
        lastWasPressed = True;
    } else {
        log_debug("discarding auto-repeated key 0x%04x (%d)", code, code);
    }
    return TRUE;
}

//
gboolean key_released(GtkWidget* widget, GdkEventKey* evt, gpointer data) {
    send_key_stroke(0, evt->hardware_keycode - 8, fdSerialPort);
    lastWasPressed = False;
    return TRUE;
}

//
typedef struct {
    char* file;
    int listen;
} thread_data;

//
void open_keyboard_window_threaded(thread_data* d) {

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), IMAGE_WINDOW_NAME);

    if (d->listen) {
        gtk_widget_add_events(window, GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
        g_signal_connect(window, "key_press_event", G_CALLBACK(key_pressed), NULL);
        g_signal_connect(window, "key_release_event", G_CALLBACK(key_released), NULL);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(window_destroy), NULL);

    GtkWidget* image = gtk_image_new_from_file(d->file);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();
}

//
void open_keyboard_window_or_die(char* file, int listen) {

    gtk_init(NULL, NULL);

    char* name = "gtk-thread";
    GError* err = NULL;
    thread_data d;
    d.file = file;
    d.listen = listen;

    GThread* thread = g_thread_try_new(
        name, (GThreadFunc)open_keyboard_window_threaded, &d, &err);

    if (thread != NULL) {
        log_debug("keyboard window thread created");
    } else {
        log_fatal("cannot open keyboard window: thread creation failed: %s",
            err->message);
        g_error_free(err);
        exit(EXIT_FAILURE);
    }
}

// --- window focus -----------------------------------------------------------

char ownWindowName[WIN_NAME_BUF_SIZE];
char bufName[WIN_NAME_BUF_SIZE];
Bool xerror = False;

//
int handle_x_error(Display* d, XErrorEvent* error) {
    log_error("X11 error");
    xerror = True;
    return 1;
}

//
Window get_focus_window(Display* d) {

    log_debug("getting input focus window");

    Window w;
    int revert_to;
    XGetInputFocus(d, &w, &revert_to);

    if (xerror) {
        log_error("failed to get focused window");
        w = None;
    } else if (w == None) {
        log_warn("no focused window");
    } else {
        log_debug("focused window: %d", (int)w);
    }

    return w;
}

//
Window get_top_window(Display* d, Window start) {

    if (start == None) {
        return None;
    }

    log_debug("getting top window");

    Window w = start;
    Window parent = start;
    Window root = None;
    Window *children;
    unsigned int nchildren;
    Status s;

    while (parent != root) {
        w = parent;
        s = XQueryTree(d, w, &root, &parent, &children, &nchildren);
        if (s) {
            XFree(children);
        }
        if (xerror) {
            log_error("failed to get top window");
            return None;
        }
        log_trace("  window: %d", (int)w);
    }

    log_debug("top window: %d", (int)w);
    return w;
}

//
Window get_named_window(Display* d, Window start) {

    if (start == None) {
        return None;
    }

    log_debug("getting named window");

    Window w = XmuClientWindow(d, start);
    if (w == start) {
        log_debug("no named window");
        return None;
    }

    log_debug("named window: %d", (int)w);
    return w;
}

// (XFetchName cannot get a name with multi-byte chars)
int get_window_name(Display* d, Window w, char* buf, int size) {

    int ret = 0;

    if (w == None) {
        return ret;
    }

    log_debug("getting window name");

    XTextProperty prop;
    Status s;

    s = XGetWMName(d, w, &prop);
    if (!xerror && s) {
        int count = 0, result;
        char **list = NULL;
        result = XmbTextPropertyToTextList(d, &prop, &list, &count);
        if (result == Success) {
            strncpy(buf, list[0], size-1);
            buf[size-1] = '\0';
            ret = 1;
            log_debug("window name: %s", buf);
        } else {
            log_error("XmbTextPropertyToTextList failed");
        }
        XFreeStringList(list);
    } else {
        log_error("XGetWMName failed");
    }

    return ret;
}

//
int get_current_window_name(Display* d, char* buf, int size) {
    Window w = get_focus_window(d);
    w = get_top_window(d, w);
    w = get_named_window(d, w);
    return get_window_name(d, w, buf, size);
}

//
int is_in_focus(Display* d, const char* ownName, char* bufName, int size) {
    return get_current_window_name(d, bufName, size)
        && strcmp(ownName, bufName) == 0;
}

//
void get_own_window_name_or_die(Display* d) {
    if (get_current_window_name(d, ownWindowName, sizeof(ownWindowName))) {
        log_info("own window name: %s", ownWindowName);
    } else {
        log_fatal("cannot get own window name");
        exit(EXIT_FAILURE);
    }
}

//
Display* open_display_or_die() {

    log_info("connecting to X server");
    setlocale(LC_ALL, ""); // for XmbTextPropertyToTextList; see man locale
    Display* d = XOpenDisplay(NULL);
    if (d == NULL) {
        log_fatal("failed to connect to X server");
        exit(EXIT_FAILURE);
    }

    XSetErrorHandler(handle_x_error);
    return d;
}

// --- keyboard ---------------------------------------------------------------

int open_keyboard_or_die(char* kbd) {
    int fd = open(kbd, O_RDONLY);
    if (fd < 0) {
        log_fatal("cannot open keyboard device %s: %s. try with sudo?",
            kbd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}

//
void close_keyboard(int fd) {
    if (fd) {
        log_info("closing keyboard");
        close(fd);
    }
}

// --- read & send loop -------------------------------------------------------

// read key strokes & send to serial; NULL display implies to always read key events
void read_kbd_and_send(Display* d, int fdKbd, int fdSer) {

    log_info("starting to read from keyboard");

    struct input_event ev;
    ssize_t n;

    while (TRUE) {

        n = read(fdKbd, &ev, sizeof ev);

        if (d != NULL &&
            !is_in_focus(d, ownWindowName, bufName, sizeof(ownWindowName))) {
            log_trace("not in focus");
            continue;
        }

        if (n == (ssize_t)-1) {
            if (errno == EINTR) {
                continue;
            } else {
                break;
            }
        } else if (n != sizeof ev) {
            errno = EIO;
            break;
        }

        if (ev.type == EV_KEY) {
            send_key_stroke(ev.value, ev.code, fdSer);
        }
    }
}


// --- main -------------------------------------------------------------------

//
void usage() {
    printf("\nsynopsis:\n\n  kev \
-p {serial port device} [-i {keyboard image file}] [-k {keyboard device}] [-a] \
[-v debug|trace]\n\n\
    -i  open new window with given image file and listen for key events there;\n\
        does not require root privileges, and all key event sources of the\n\
        system will be considered, i.e. all attached keyboards, but also game\n\
        controllers creating key events via e.g. QJoyPad\n\n\
    -k  read key events only from the given keyboard device; implied when not\n\
        using -i; requires root privileges\n\n\
    -a  read all key events, regardless of whether console window is in focus;\n\
        implies -k\n\n\
    -v  log level, 'debug' or 'trace'\n\n");
    exit(EXIT_SUCCESS);
}

//
void cleanup() {
    close_keyboard(fdKeyboard);
    send_key_stroke('!', 0, fdSerialPort); // reset adapter
    close_serial_port(fdSerialPort);
}

//
void sigIntHandler() {
    cleanup();
    log_info("exiting");
    exit(0);
}

//
int main(int argc, char* argv[]) {

    log_set_level(LOG_INFO);

    if (argc == 1) {
        usage();
    }

    char* devKbd = NULL;
    char* imgKbd = NULL;
    char* portName = NULL;
    int useDisplay = 1;

    int opt;
    while((opt = getopt(argc, argv, ":hk:i:p:lv:")) != -1) {
        switch(opt) {

            case 'h':
                usage();
                break;

            case 'k': // keyboard (optional)
                devKbd = optarg;
                break;

            case 'i': // keyboard image (optional)
                imgKbd = optarg;
                break;

            case 'p': // serial port (required)
                portName = optarg;
                break;

            case 'l': // no display (optional)
                useDisplay = 0;
                break;

            case 'v': // log level
                if (strcmp("debug", optarg) == 0) {
                    log_set_level(LOG_DEBUG);
                } else if (strcmp("trace", optarg) == 0) {
                    log_set_level(LOG_TRACE);
                } else {
                    log_fatal(
                        "unknown log level: '%s', use 'debug' or 'trace', omit for 'info'",
                        optarg);
                    return EXIT_FAILURE;
                }
                break;

            case ':':
                log_fatal("option needs a value");
                return EXIT_FAILURE;

            case '?':
                log_fatal("unknown option: %c", optopt);
                return EXIT_FAILURE;
        }
    }

    if (imgKbd == NULL && devKbd == NULL) {
        devKbd = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    }

    if (imgKbd != NULL && !useDisplay) {
        log_fatal("-l conflicts with -i");
        return EXIT_FAILURE;
    }

    signal(SIGINT, sigIntHandler);

    fdSerialPort = open_serial_port_or_die(portName);

    Display* disp = NULL;
    if (useDisplay) {
        disp = open_display_or_die();
    }

    if (imgKbd != NULL) {
        open_keyboard_window_or_die(imgKbd, devKbd == NULL);
        strncpy(ownWindowName, IMAGE_WINDOW_NAME, sizeof(ownWindowName)-1);
    } else {
        if (useDisplay) {
            get_own_window_name_or_die(disp);
        } else {
            log_info("disregarding input focus");
        }
    }

    if (devKbd == NULL) {
        log_info("listening for key events on image window");
        sigset_t mask;
        sigemptyset(&mask);
        sigsuspend(&mask);
    } else {
        log_info("reading key events from keyboard %s", devKbd);
        fdKeyboard = open_keyboard_or_die(devKbd);
        read_kbd_and_send(disp, fdKeyboard, fdSerialPort);
        // read_xevents_and_send(disp, fdSerialPort);
    }

    cleanup();
    fflush(stdout);
    log_fatal("%s", strerror(errno));
    return EXIT_FAILURE;
}
