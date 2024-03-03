import argparse, sys
from serial import Serial
import logging

logging.basicConfig(level=logging.INFO)
# Implicit, 48 bytes
cmd_set_lora_modes = bytes([0x40, 0x53, 0x0A, 0x08, 0x00, \
                            0x01, 0x30, 0x01, 0x01, 0x00, 0x08, 0x00, 0x55])
# 434 MHz, BW_125, SF9, CR_4_8
cmd_set_lora_params = bytes([0x40, 0x53, 0x01, 0x07, 0x00, \
                             0x80, 0x50, 0xDE, 0x19, 0x07, 0x09, 0x04])

def recv_msg(ser):
    got_msg = False
    while not (got_msg):
        # minimum packet is 5 bytes
        pkt = ser.read(5)
        if len(pkt) < 5:
            continue
        if pkt[0] != 0x40 & pkt[1] != 0x53:
            continue
        payload_len = pkt[4] << 8 | pkt[3]
        pkt += ser.read(payload_len)
        logging.info("Received: %s", pkt.hex(' ').upper())
        got_msg = True
        # command, payload
        return pkt[2], pkt[5:]

def main():
    parser = argparse.ArgumentParser(description="Host-side receiver")
    required = parser.add_argument_group('required arguments')
    required.add_argument("-s", "--serport", help="LoRa module serial port name", required=True)
    args = parser.parse_args()
    ser = Serial(args.serport, 115200)
    ser.write(cmd_set_lora_modes)
    logging.info("Sent: %s", cmd_set_lora_modes.hex(' ').upper())
    recv_msg(ser)
    ser.write(cmd_set_lora_params)
    logging.info("Sent: %s", cmd_set_lora_params.hex(' ').upper())
    recv_msg(ser)
    logging.info("Wait for air packet reception...")
    recv_msg(ser)
    ser.close()

if __name__ == "__main__":
    main()
