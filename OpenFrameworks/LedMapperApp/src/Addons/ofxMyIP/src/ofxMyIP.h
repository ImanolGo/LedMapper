/*
 The MIT License (MIT)
 
 Copyright (c) 2015 Satoshi HORII ( http://satcy.net/ )
 
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

#pragma once

#include "ofMain.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

struct ofxMyIPAddress {
    string name;
    string address;
    string netmask;
    string broadcast;
};

class ofxMyIP
{
protected:
    vector<ofxMyIPAddress> list;
public:
    
    void setup(){
		struct ifaddrs *myaddrs;
		struct ifaddrs *ifa;
		struct sockaddr_in *s4;
		int status;
		
		char buf[INET_ADDRSTRLEN];
		
		status = getifaddrs(&myaddrs);
		if (status != 0) {
            ofLog(OF_LOG_NOTICE, "getifaddrs");
		}
		
		for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL) continue;
			if ((ifa->ifa_flags & IFF_UP) == 0) continue;
			
			if (ifa->ifa_addr->sa_family == AF_INET) {
				s4 = (struct sockaddr_in *)(ifa->ifa_addr);
				if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL) {
					printf("%s: inet_ntop failed!\n", ifa->ifa_name);
				} else {
                    ofxMyIPAddress addr;
                    addr.name = ofToString(ifa->ifa_name);
                    addr.address = ofToString(buf);
                    
                    s4 = (struct sockaddr_in *)(ifa->ifa_netmask);
                    if (inet_ntop(ifa->ifa_netmask->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL) {
                        addr.netmask = "";
                    } else {
                        addr.netmask = ofToString(buf);
                    }
                    
                    s4 = (struct sockaddr_in *)(ifa->ifa_broadaddr);
                    if (inet_ntop(ifa->ifa_broadaddr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL) {
                        addr.broadcast = "";
                    } else {
                        addr.broadcast = ofToString(buf);
                    }
                    
                    
                    list.push_back(addr);
				}
			}
		}
		
		freeifaddrs(myaddrs);
    }
    
    const vector<ofxMyIPAddress> getList(){
        return list;
    }
    
    string getIpAddress(){
        return getIpAddress("en0");
    }
    
	string getIpAddress(string interface)
	{
        if ( list.empty() ) return "";
        int find = -1;
        for ( int i=0; i<list.size(); i++ ) {
            if ( list[i].name == interface ) {
                find = i;
                break;
            }
        }
        if ( find != -1 ) {
            return list[find].address;
        } else {
            return "";
        }
	}
};