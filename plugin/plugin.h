/*

  Copyright (c) 2009-2010 Samuel Lidén Borell <samuel@slbdata.se>
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <X11/X.h>

typedef enum {
    PT_Version,
    PT_Authentication,
    PT_Signer,
} PluginType;

typedef enum {
    PE_OK = 0,
    PE_UnknownError = 1, // Maybe this is used for something else in the original plugin?
} PluginError;

typedef struct {
    PluginType type;
    
    char *url;
    char *hostname;
    char *ip;
    Window windowId;
    PluginError lastError;
    
    union {
        struct {
            /* Input parameters */
            char *challenge;
            char *policys;
            char *subjectFilter;
            void *dummy0, *dummy1; // To be compatible with .sign below
            /* Output parameters */
            char *signature;
        } auth;
        struct {
            /* Input parameters */
            char *challenge;
            char *policys;
            char *subjectFilter;
            char *message;
            char *invisibleMessage;
            /* Output parameters */
            char *signature;
        } sign;
    } info;
} Plugin;

/* Plugin creation */
Plugin *plugin_new(PluginType pluginType, const char *url,
                   const char *hostname, const char *ip,
                   Window windowId);
void plugin_free(Plugin *plugin);

/* Javascript API */
char *version_getVersion(Plugin *plugin);

char *sign_getParam(Plugin *plugin, const char *name);
bool sign_setParam(Plugin *plugin, const char *name, const char *value);
int sign_performAction(Plugin *plugin, const char *action);
int sign_performAction_Authenticate(Plugin *plugin);
int sign_performAction_Sign(Plugin *plugin);
int sign_getLastError(Plugin *plugin);
// TODO more functions...

#endif


