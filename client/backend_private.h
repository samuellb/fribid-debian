/*

  Copyright (c) 2010-2011 Samuel Lidén Borell <samuel@slbdata.se>
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, token to the following conditions:
  
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

#ifndef BACKEND_PRIVATE_H
#define BACKEND_PRIVATE_H

#include <stddef.h>
#include <stdbool.h>
#include "backend.h"

#ifndef TokenType
    #define TokenType Token
#endif
#ifndef BackendPrivateType
    #define BackendPrivateType void
#endif


typedef struct Backend Backend;
struct Backend {
    BackendPrivateType *private;
    const BackendNotifier *notifier;
    
    /* Initialization and de-initialization */
    bool (*init)(Backend *backend);
    void (*free)(Backend *backend);
    
    /**
     * Free's a token.
     */
    void (*freeToken)(TokenType *token);
    
    /**
     * Scan tokens provided by this backend
     */
    void (*scan)(Backend *backend);

    /**
     * Manually adds a file to the backend. May be NULL if not applicable
     */
    TokenError (*addFile)(Backend *backend, const char *data, size_t length,
                          void *tag);
                              
    /**
     * Generates a key pair and creates a certificate request for it.
     *
     * TODO change the backend interface to support pinpads too
     */
    TokenError (*createRequest)(const RegutilInfo *info,
                                const char *hostname,
                                const char *password,
                                char **request, size_t *reqlen);
    
    TokenError (*storeCertificates)(const char *p7data, size_t length,
                                    const char *hostname);
    
    
    TokenError (*getBase64Chain)(const TokenType *token,
                                 char ***certs, size_t *count);
    TokenError (*sign)(TokenType *token,
                       const char *message, size_t messagelen,
                       char **signature, size_t *siglen);
};

struct Token {
    const Backend *backend;
    TokenError lastError;
    TokenStatus status;
    
    bool isManuallyAdded;
    char *displayName;
    void *tag;
    const char *password;
};

struct BackendNotifier {
    size_t backendCount;
    Backend **backends;
    
    char *subjectFilter;
    KeyUsage keyUsage;
    BackendNotifyFunction notifyFunction;
};

#endif


