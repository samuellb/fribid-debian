/*

  Copyright (c) 2009 Samuel Lidén Borell <samuel@slbdata.se>
 
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

#define _BSD_SOURCE 1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "backend.h"
#include "xmldsig.h"
#include "misc.h"

static const char xmldsig_template[] = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\">"
        "%s"
        "<SignatureValue>%s</SignatureValue>"
        "%s"
        "<Object>%s</Object>"
    "</Signature>";

static const char signedinfo_template[] =
    "<SignedInfo xmlns=\"http://www.w3.org/2000/09/xmldsig#\">"
        "<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\">"
        "</CanonicalizationMethod>"
        "<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha1\">"
        "</SignatureMethod>"
        "<Reference Type=\"http://www.bankid.com/signature/v1.0.0/types\" URI=\"#bidSignedData\">"
            "<Transforms>"
                "<Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"></Transform>"
            "</Transforms>"
            "<DigestMethod Algorithm=\"http://www.w3.org/2001/04/xmlenc#sha256\"></DigestMethod>"
            "<DigestValue>%s</DigestValue>"
        "</Reference>"
        "<Reference URI=\"#bidKeyInfo\">"
            "<Transforms>"
                "<Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"></Transform>"
            "</Transforms>"
            "<DigestMethod Algorithm=\"http://www.w3.org/2001/04/xmlenc#sha256\"></DigestMethod>"
            "<DigestValue>%s</DigestValue>"
        "</Reference>"
    "</SignedInfo>";


static const char keyinfo_template[] =
    "<KeyInfo xmlns=\"http://www.w3.org/2000/09/xmldsig#\" Id=\"bidKeyInfo\">"
        "<X509Data>%s</X509Data>"
    "</KeyInfo>";

static const char cert_template[] =
    "<X509Certificate>%s</X509Certificate>";

/**
 * Creates a xmldsig signature. See the sign function in bankid.c.
 */
char *xmldsig_sign(Token *token, const char *dataId, const char *data) {
    
    // Keyinfo
    char **certs;
    size_t certCount;
    if (!token_getBase64Chain(token, &certs, &certCount)) {
        return NULL;
    }
    
    size_t certsLength = (strlen(cert_template)-2) * certCount;
    for (size_t i = 0; i < certCount; i++) {
        certsLength += strlen(certs[i]);
    }
    
    char *keyinfoInner = malloc(certsLength+1);
    keyinfoInner[0] = '\0';
    char *keyend = keyinfoInner;
    for (size_t i = 0; i < certCount; i++) {
        keyend += sprintf(keyend, cert_template, certs[i]);
        free(certs[i]);
    }
    free(certs);
    
    char *keyinfo = rasprintf(keyinfo_template, keyinfoInner);
    free(keyinfoInner);
    
    // SignedInfo
    char *data_sha = sha_base64(data);
    char *keyinfo_sha = sha_base64(keyinfo);
    
    char *signedinfo = rasprintf(signedinfo_template, data_sha, keyinfo_sha);
    free(keyinfo_sha);
    free(data_sha);
    
    
    // Signature
    char *sigData;
    size_t sigLen;
    
    if (!token_sign(token, signedinfo, strlen(signedinfo),
                    &sigData, &sigLen)) {
        free(keyinfo);
        free(signedinfo);
        return NULL;
    }
    
    char *signature = base64_encode(sigData, sigLen);
    free(sigData);
    
    // Glue everything together
    char *complete = rasprintf(xmldsig_template,
                               signedinfo, signature, keyinfo, data);
    
    free(keyinfo);
    free(signedinfo);
    free(signature);
    
    return complete;
}

