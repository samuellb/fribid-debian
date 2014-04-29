/*

  Copyright (c) 2011 Samuel Lidén Borell <samuel@kodafritt.se>
 
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

#ifndef CERTUTIL_H
#define CERTUTIL_H

// Certificate utilities

#include <stdbool.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>

#include "../common/bidtypes.h"

extern const int opensslKeyUsages[];

X509_NAME *certutil_parse_dn(const char *s, bool fullDN);
char *certutil_derEncode(X509 *cert);
bool certutil_hasKeyUsage(X509 *cert, KeyUsage keyUsage);
char *certutil_getNamePropertyByNID(X509_NAME *name, int nid);
char *certutil_getDisplayNameFromDN(X509_NAME *xname);
bool certutil_matchSubjectFilter(const char *subjectFilter, X509_NAME *name);
bool certutil_compareX509Names(const X509_NAME *a, const X509_NAME *b,
                               bool orderMightDiffer);
X509 *certutil_findCert(const STACK_OF(X509) *certList,
                        const X509_NAME *name,
                        const KeyUsage keyUsage,
                        bool orderMightDiffer);
bool certutil_addToList(char ***list, size_t *count, X509 *cert);
void certutil_freeList(char ***list, size_t *count);
PKCS7 *certutil_parseP7SignedData(const char *p7data, size_t length);
char *certutil_makeFilename(X509_NAME *xname);
char *certutil_getBagAttr(PKCS12_SAFEBAG *bag, ASN1_OBJECT *oid);
void certutil_clearErrorString(void);
void certutil_updateErrorString(void);
char *certutil_getErrorString(void);

#endif

