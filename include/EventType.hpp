#pragma once

namespace Capefox {
    enum EventType {
        Unknown,
        Alert,
        Anomaly,
        Dcerpc,
        Dns,
        Dhcp,
        Fileinfo,
        Flow,
        Http,
        Krb5,
        Smb,
        Stat,
        Tls
    };
};