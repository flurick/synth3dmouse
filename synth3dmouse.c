// Attempt to make a broken 3d mouse "SpacePilot" usable as normal keyboard keys,
// in programs under linux/wayland unaware of the 3d mouse keycodes


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <spnav.h> //read 3dmouse events via the opensource drivers
#include <libevdev/libevdev.h> //access input devices
#include <libevdev/libevdev-uinput.h>


void sig(int s)
{
	spnav_close();
	exit(0);
}

int main(void)
{

	spnav_event sev;

	signal(SIGINT, sig);

	if(spnav_open()==-1) {
	  	fprintf(stderr, "failed to connect to the space navigator daemon\n");
		return 1;
	}


	// Construct new device
	// from: https://www.freedesktop.org/software/libevdev/doc/latest/group__uinput.html
	int err;
	struct libevdev *dev;
	struct libevdev_uinput *uidev;
	 
    dev = libevdev_new();
	libevdev_set_name(dev, "synthetic 3D mouse");

    //linux keycodes in https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

    libevdev_enable_event_code(dev, EV_KEY, KEY_1, NULL); /*should probably be KEY_MACRO1 .. KEY_MACRO6 instead */
    libevdev_enable_event_code(dev, EV_KEY, KEY_2, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_3, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_4, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_5, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_6, NULL);

    //libevdev_enable_event_type(dev, EV_REL);
    /*todo: map to some optional joystick maybe from the "Absolute axes" list*/
    //libevdev_enable_event_code(dev, EV_REL, REL_X, NULL);
    //libevdev_enable_event_code(dev, EV_REL, REL_Y, NULL);

	libevdev_enable_event_type(dev, EV_KEY);
	libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
	libevdev_enable_event_code(dev, EV_KEY, BTN_MIDDLE, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_SIDE, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_EXTRA, NULL);

    libevdev_enable_event_code(dev, EV_KEY, KEY_ESC, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_LEFTCTRL, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_LEFTALT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, KEY_LEFTSHIFT, NULL);

    // "-" button
    libevdev_enable_event_code(dev, EV_KEY, KEY_KPPLUS, NULL);
    // "Dom" button

	err = libevdev_uinput_create_from_device(dev,
	                                         LIBEVDEV_UINPUT_OPEN_MANAGED,
	                                         &uidev);
	if (err != 0)
	    return err;
	 




	/* spnav_wait_event() and spnav_poll_event(), will silently ignore any non-spnav X11 events.
	 *
	 * If you need to handle other X11 events you will have to use a regular XNextEvent() loop,
	 * and pass any ClientMessage events to spnav_x11_event, which will return the event type or
	 * zero if it's not an spnav event (see spnav.h).
	 */
	while(spnav_wait_event(&sev)) {
		if(sev.type == SPNAV_EVENT_MOTION) {
            //printf("got motion event: t(%d, %d, %d) ", sev.motion.x, sev.motion.y, sev.motion.z);
            //printf("r(%d, %d, %d)\n", sev.motion.rx, sev.motion.ry, sev.motion.rz);
        } else {	/* SPNAV_EVENT_BUTTON */


            //printf("%d\n", sev.button.bnum);


            switch (sev.button.bnum) {

                case  0: libevdev_uinput_write_event(uidev, EV_KEY, KEY_1, sev.button.press); break;
                case  1: libevdev_uinput_write_event(uidev, EV_KEY, KEY_2, sev.button.press); break;
                case  2: libevdev_uinput_write_event(uidev, EV_KEY, KEY_3, sev.button.press); break;
                case  3: libevdev_uinput_write_event(uidev, EV_KEY, KEY_4, sev.button.press); break;
                case  4: libevdev_uinput_write_event(uidev, EV_KEY, KEY_5, sev.button.press); break;
                case  5: libevdev_uinput_write_event(uidev, EV_KEY, KEY_6, sev.button.press); break;

                case  10: //Esc key
                    libevdev_uinput_write_event(uidev, EV_KEY, KEY_ESC, sev.button.press);
                break;
                case  13: //Ctrl key
                    libevdev_uinput_write_event(uidev, EV_KEY, KEY_LEFTCTRL, sev.button.press);
                break;
                case  11: //Alt key
                    libevdev_uinput_write_event(uidev, EV_KEY, KEY_LEFTALT, sev.button.press);
                break;
                case  12: //Shift key
                    libevdev_uinput_write_event(uidev, EV_KEY, KEY_LEFTSHIFT, sev.button.press);
                break;

                case 15: //"Config" button
                libevdev_uinput_write_event(uidev, EV_KEY, BTN_SIDE, sev.button.press);
                break;
                case 20: //"Panel" button
                libevdev_uinput_write_event(uidev, EV_KEY, BTN_EXTRA, sev.button.press);
                break;

                case 16: //dom "+" button
                libevdev_uinput_write_event(uidev, EV_KEY, KEY_KPPLUS, sev.button.press);
                break;

                case 6: //thumb T button
                    libevdev_uinput_write_event(uidev, EV_KEY, BTN_RIGHT, sev.button.press);
                break;
                case 7: //thumb L button
                    libevdev_uinput_write_event(uidev, EV_KEY, BTN_LEFT, sev.button.press);
                break;
                case 19: //thumb center "3D lock" button
                    libevdev_uinput_write_event(uidev, EV_KEY, BTN_MIDDLE, sev.button.press);
                    break;
                case 14: //thumb "fit" button
                    libevdev_uinput_write_event(uidev, EV_KEY, BTN_SIDE, sev.button.press);
                    break;

                default:
                    printf("got un-mapped button %s event b(%d)\n", sev.button.press ? "press" : "release", sev.button.bnum);

            }
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
		}
	}

	spnav_close();
	libevdev_uinput_destroy(uidev);
	return 0;
}
