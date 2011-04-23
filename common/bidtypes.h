/*

  Copyright (c) 2011 Samuel Lidén Borell <samuel@slbdata.se>
 
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

#ifndef __BIDTYPES_H__
#define __BIDTYPES_H__

#include <stdbool.h>

typedef enum {
    KeyUsage_Issuing,
    KeyUsage_Signing,
    KeyUsage_Authentication,
} KeyUsage;

// regutil requests
typedef struct PKCS10Request {
    struct PKCS10Request *next;
    
    KeyUsage keyUsage;
    int keySize;
    char *subjectDN;
    bool includeFullDN;
} RegutilPKCS10;

typedef struct CMCRequest {
    struct CMCRequest *next;
    
    char *oneTimePassword;
    char *rfc2729cmcoid;
} RegutilCMC;

typedef struct {
    int minPasswordLength;
    int minPasswordNonDigits;
    int minPasswordDigits;
    
    RegutilPKCS10 *pkcs10;
    RegutilCMC cmc;
} RegutilInfo;

#endif

