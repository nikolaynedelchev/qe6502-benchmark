#!/usr/bin/env python3
"""
Convert AS65/Intel HEX output to the 64KB C-style byte-array HEX format used by the test harness.

Example:
    python ihex_to_h_hex.py 6502_functional_test_nes_no_dflag.hex

Output:
    6502_functional_test_nes_no_dflag_h.hex

Output format:
    {
    0x00,0x00,... 32 bytes per line ...
    };
"""

from __future__ import annotations

import argparse
from pathlib import Path


MEM_SIZE = 0x10000
BYTES_PER_LINE = 32


class IntelHexError(Exception):
    pass


def parse_intel_hex(path: Path) -> tuple[bytearray, int | None, int | None]:
    """
    Parse Intel HEX into a 64KB image.

    Policy matched to the provided harness-format file:
      - default fill is 0xff
      - bytes before the first data record address are forced to 0x00
        (AS65 starts this test at address 0x000A, and the reference harness
         image has 0x00 at addresses 0x0000..0x0009)
      - data records overwrite the image at their absolute addresses
      - EOF address is returned as start_address when AS65 encodes one
    """
    image = bytearray([0xFF] * MEM_SIZE)

    min_data_addr: int | None = None
    start_address: int | None = None
    upper_base = 0

    lines = path.read_text(encoding="ascii").splitlines()

    for line_no, raw_line in enumerate(lines, start=1):
        line = raw_line.strip()
        if not line:
            continue

        if not line.startswith(":"):
            raise IntelHexError(f"{path}:{line_no}: line does not start with ':'")

        try:
            record = bytes.fromhex(line[1:])
        except ValueError as exc:
            raise IntelHexError(f"{path}:{line_no}: invalid hex characters") from exc

        if len(record) < 5:
            raise IntelHexError(f"{path}:{line_no}: record too short")

        byte_count = record[0]
        address = (record[1] << 8) | record[2]
        record_type = record[3]
        data = record[4:4 + byte_count]

        if len(data) != byte_count or len(record) != 5 + byte_count:
            raise IntelHexError(f"{path}:{line_no}: byte count does not match record length")

        if (sum(record) & 0xFF) != 0:
            raise IntelHexError(f"{path}:{line_no}: bad checksum")

        if record_type == 0x00:  # Data
            absolute = upper_base + address
            end = absolute + byte_count
            if absolute < 0 or end > MEM_SIZE:
                raise IntelHexError(
                    f"{path}:{line_no}: data range ${absolute:04X}..${end - 1:04X} "
                    f"is outside 64KB"
                )

            image[absolute:end] = data

            if min_data_addr is None or absolute < min_data_addr:
                min_data_addr = absolute

        elif record_type == 0x01:  # End Of File
            # AS65 writes ':00040001FB' for this test, so keep the EOF address.
            start_address = address
            break

        elif record_type == 0x02:  # Extended Segment Address
            if byte_count != 2:
                raise IntelHexError(f"{path}:{line_no}: bad type 02 record length")
            upper_base = int.from_bytes(data, "big") << 4

        elif record_type == 0x04:  # Extended Linear Address
            if byte_count != 2:
                raise IntelHexError(f"{path}:{line_no}: bad type 04 record length")
            upper_base = int.from_bytes(data, "big") << 16

        elif record_type in (0x03, 0x05):
            # Start address records. Not needed for this 6502 binary.
            pass

        else:
            raise IntelHexError(f"{path}:{line_no}: unsupported record type {record_type:02X}")

    if min_data_addr is not None:
        for i in range(min_data_addr):
            image[i] = 0x00

    return image, min_data_addr, start_address


def output_path_for(input_path: Path) -> Path:
    if input_path.suffix.lower() != ".hex":
        return input_path.with_name(input_path.name + "_h.hex")
    return input_path.with_name(input_path.stem + "_h" + input_path.suffix)


def write_harness_hex(path: Path, image: bytes) -> None:
    parts: list[str] = ["", "{"]

    for offset in range(0, len(image), BYTES_PER_LINE):
        chunk = image[offset:offset + BYTES_PER_LINE]
        line_items = []

        for i, value in enumerate(chunk):
            absolute_index = offset + i
            item = f"0x{value:02x}"
            if absolute_index != len(image) - 1:
                item += ","
            line_items.append(item)

        parts.append("".join(line_items))

    parts.append("};")
    parts.append("")

    path.write_text("\n".join(parts), encoding="ascii")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Convert AS65/Intel HEX to 64KB harness .hex format."
    )
    parser.add_argument("input_hex", type=Path, help="Input AS65/Intel HEX file")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Output file. Defaults to inserting _h before .hex",
    )
    args = parser.parse_args()

    input_path = args.input_hex
    output_path = args.output or output_path_for(input_path)

    image, min_data_addr, start_address = parse_intel_hex(input_path)
    write_harness_hex(output_path, image)

    print(f"input:        {input_path}")
    print(f"output:       {output_path}")
    print(f"bytes:        {len(image)}")
    print(f"first record: ${min_data_addr:04X}" if min_data_addr is not None else "first record: none")
    print(f"start addr:   ${start_address:04X}" if start_address is not None else "start addr:   unknown")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
