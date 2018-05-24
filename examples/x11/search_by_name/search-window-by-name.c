#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include <X11/Xlib.h>           // for Display structure
#include <X11/Xmu/WinUtil.h>    

#define _NET_WM_STATE_REMOVE    0
#define _NET_WM_STATE_ADD       1
#define _NET_WM_STATE_TOGGLE    2

Bool xerror = False;

Display* openDisplay() {
    printf("connecting to X server....");
    Display *d = XOpenDisplay(NULL);
    if(d == NULL) {
        printf("failed\n");
    } else {
        printf("success\n");
    }
    return d;
}

Window getNamedWindow(Display *d, Window current) {
    Window w;
    w = XmuClientWindow(d, current);
    // printf("Current [%d] \n" , (int) current);
    return w;
}

void setOnTop(Display *d, Window current) {
    
     XClientMessageEvent xclient;
        memset(&xclient, 0 , sizeof(xclient));

        xclient.type = ClientMessage;
        xclient.window = current;
        xclient.message_type = XInternAtom(d, "_NET_ACTIVE_WINDOW", True);;
        xclient.format = 32;
        xclient.data.l[0] = 1;
        xclient.data.l[1] = CurrentTime;
        xclient.data.l[2] = 0;
        xclient.data.l[3] = 0;
        xclient.data.l[4] = 0;

        XSetInputFocus(d, current, None, CurrentTime);
        XSendEvent(d, DefaultRootWindow(d), False, SubstructureNotifyMask|SubstructureRedirectMask, (XEvent *)&xclient);
        XFlush(d);   

}

void searchAndDisplay(Display *d, Window current, const char *title) {

    XTextProperty prop;
    Status s;
    
    s = XGetWMName(d, current, &prop);
    if(s) {
        int count = 0, result;
        char **list = NULL;
        result = XmbTextPropertyToTextList(d, &prop, &list, &count);
        if(result == Success) {
            if(count > 0) {
                XWindowAttributes attr;
                XGetWindowAttributes(d, current, &attr);
                if(attr.map_state == 2) {
                    printf("Title: %s\n", list[0]);
                    if(strcmp(title, list[0]) == 0) {
                        setOnTop(d, current);
                    }
                    Atom *props;
                    int numOfProp;
                    props = XListProperties(d, current, &numOfProp);
                    for(int i = 0; i < numOfProp; i++) {
                        // if(strcmp(XGetAtomName(d, props[i]), "_NET_WM_WINDOW_TYPE") == 0) {
                        //     printf("Window type: %ld | ", props[i]);
                        // }
                        printf("\tName: %s | Value:%ld\n", XGetAtomName(d, props[i]), props[i]);
                        
                    }
                }
                
                
                
            }
        } else {
            // printf("Error: XmbTextPropertyToTextList\n");
        }
    } 
    else {
        // printf("Error: XGetWMName\n");
    }
}



int main(int argc, const char *argv[]) {

    setlocale(LC_ALL, "");

    Display *d = openDisplay();
    Window root = XDefaultRootWindow(d);
    printf("root: %d\n", (int) root);

    Window parent;
    Window *children;
    unsigned int nChildren;

    int status = XQueryTree(d, root, &root, &parent, &children, &nChildren);
    if(status == 0) {
        return 0;
    }
    if(nChildren == 0) 
        return 0;

    for(int i = 0; i < nChildren; i++) {
        Window cur = getNamedWindow(d, children[i]);
        searchAndDisplay(d, cur, argv[1]);
    }
    XFree(children);
}