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

/*
    serial port code based on:
        https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c

    window focus code based on:
        https://gist.github.com/kui/2622504

    log library:
        https://github.com/rxi/log.c

 */

// --- keyboard image window --------------------------------------------------

#define IMAGE_WINDOW_NAME "Spectratur"

//
void keyboard_window_destroy(void) {
    gtk_main_quit();
    log_info("exiting");
    exit(EXIT_SUCCESS);
}

//
void open_keyboard_window_threaded(char* file) {

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), IMAGE_WINDOW_NAME);
    g_signal_connect(window, "destroy",
        G_CALLBACK(keyboard_window_destroy), NULL);

    GtkWidget* image = gtk_image_new_from_file(file);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();
}

//
void open_keyboard_window_or_die(char* file) {

    gtk_init(NULL, NULL);

    if (g_thread_supported()) {
        log_debug("g_thread supported");
    } else {
        log_fatal("cannot open keyboard window: g_thread not supported");
        exit(EXIT_FAILURE);
    }

    char *name = "gtk-thread";
    GError *err = NULL;
    GThread *thread = g_thread_try_new(
        name, (GThreadFunc)open_keyboard_window_threaded, file, &err);
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

#define WIN_NAME_BUF_SIZE 500

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


// --- serial port ------------------------------------------------------------

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


// --- read & send loop -------------------------------------------------------

//
static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

// read key strokes & send to serial
void read_kbd_and_send(Display* d, int fdKbd, int fdSer) {

    struct input_event ev;
    ssize_t n;
    char sendBuf[2];

    while (TRUE) {

        n = read(fdKbd, &ev, sizeof ev);

        if (!is_in_focus(d, ownWindowName, bufName, sizeof(ownWindowName))) {
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

        if (ev.type == EV_KEY && 0 <= ev.value && ev.value <= 2) {

            log_debug("%s 0x%04x (%d)",
                evval[ev.value], (int)ev.code, (int)ev.code);

            sendBuf[0] = (char)ev.value; // 0 = break, 1 = make
            sendBuf[1] = (char)ev.code;

            switch (ev.value) {
                case 0:
                case 1:
                    log_debug("sending to serial: [0x%x, 0x%x]",
                        sendBuf[0], sendBuf[1]);
                    write(fdSer, &sendBuf, 2);
                default: // i.e. 2, autorepeat
                    break;
            }
        }
    }
}


// --- main -------------------------------------------------------------------

//
void usage() {
    printf("\nsynopsis: kev -p {serial port device} [-k {keyboard device}] [-v debug|trace] [-i {keyboard image file}]\n\n");
    exit(EXIT_SUCCESS);
}

//
int main(int argc, char *argv[]) {

    log_set_level(LOG_INFO);

    if (argc == 1) {
        usage();
    }

    char *devKbd = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    char *imgKbd = NULL;
    char *portName = NULL;

    int opt;
    while((opt = getopt(argc, argv, ":hk:i:p:v:")) != -1) {
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

    Display* disp = open_display_or_die();
    int fdKbd = open_keyboard_or_die(devKbd);
    int fdSer = open_serial_port_or_die(portName);

    if (imgKbd != NULL) {
        open_keyboard_window_or_die(imgKbd);
        strncpy(ownWindowName, IMAGE_WINDOW_NAME, sizeof(ownWindowName)-1);
    } else {
        get_own_window_name_or_die(disp);
    }

    read_kbd_and_send(disp, fdKbd, fdSer);

    fflush(stdout);
    log_fatal("%s", strerror(errno));
    return EXIT_FAILURE;
}
