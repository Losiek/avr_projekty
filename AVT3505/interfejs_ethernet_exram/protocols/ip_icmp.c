
//Definicja nag³ówka ip
typedef struct {
    unsigned char ip_vhl;			//Version and header length.
    unsigned char ip_tos;			//Type of service.
    unsigned short ip_len;			//Total length.
    unsigned short ip_id;			//Identification.
    unsigned short ip_off;			//Fragment offset field.
    unsigned char ip_ttl;			//Time to live.
    unsigned char ip_p;				//Protocol.
    unsigned short ip_sum;			//Checksum.
    unsigned long ip_src;			//Source IP address.
    unsigned long ip_dst;			//Destination IP address.
} IPHDR;

//Definicja nag³ówka icmp
typedef struct {
    unsigned char  icmp_type;		//Type of message.
    unsigned char  icmp_code;		//Type sub code.
    unsigned short icmp_cksum;		//Ones complement header checksum.
    unsigned long  icmp_spec;		//Type specific information.
} ICMPHDR;

unsigned short IpChkSumPartial(unsigned short ics, const void *buf, int len)
{
    register unsigned long sum = ics;
    register unsigned short *wp = (unsigned short *) buf;

    // Sum up 16 bit values.
    while (len > 1) {
        sum += *wp++;
        len -= 2;
    }

    // Add remaining byte on odd lengths.
    if (len) {
        sum += *(unsigned char *) wp;
    }

    // Fold upper 16 bits to lower ones.
    while (sum >> 16) {
        sum = (unsigned short)sum + (sum >> 16);
    }
    return (unsigned short) sum;
}

//Calculates an the final IP checksum over a block of data.

unsigned short IpChkSum(unsigned short ics, const void *buf, int len)
{
    return ~IpChkSumPartial(ics, buf, len);
}

int AssembleIcmpEchoReply(void)
{
    IPHDR *ip;
    ICMPHDR *icmp;
    unsigned short csum;
    unsigned short dlen;

    if (packet[12] != 0x08 || packet[13] != 0x00) {
        puts("100 Not Internet Protocol");
        return -1;
    }

    ip = (IPHDR *)(packet + 14);
    if (ip->ip_vhl != 0x45) {
        puts("100 Not version 4");
        return -1;
    }
    if (ip->ip_p != 0x01) {
        puts("100 Not ICMP");
        return -1;
    }
    if (memcmp(&ip->ip_dst, my_ip, 4)) {
        puts("100 Not my IP address");
        return -1;
    }

    icmp = (ICMPHDR *)(packet + 34);
    if (icmp->icmp_type != 8) {
        puts("100 No echo request");
        return -1;
    }

    // Move source's to destination's MAC address.
    memcpy(packet, packet + 6, 6);
    // Source MAC address.
    memcpy(packet + 6, my_mac, 6);

    // Set IP destination and source.
    ip->ip_dst = ip->ip_src;
    memcpy(&ip->ip_src, my_ip, 4);

    // Set ICMP type.
    icmp->icmp_type = 0;

    dlen = (ip->ip_len >> 8);
    dlen += (ip->ip_len << 8);
    dlen -= sizeof(IPHDR) + sizeof(ICMPHDR);

    icmp->icmp_cksum = 0;
    csum = IpChkSumPartial(0, icmp, sizeof(ICMPHDR));
    icmp->icmp_cksum = IpChkSum(csum, packet + 42, dlen);

    return 0;
}
