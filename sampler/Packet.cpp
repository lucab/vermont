/*
 * PSAMP Reference Implementation
 *
 * Packet.cpp
 *
 * Encapsulates a captured packet with simple, thread-aware
 * reference-(usage-) counting.
 *
 * Author: Michael Drueing <michael@drueing.de>
 *
 */

#include "Packet.h"

// this is the DEFAULT value (Ethernet specific)
int Packet::IPHeaderOffset = 14;

