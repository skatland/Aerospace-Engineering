#!/usr/bin/env python3
"""Convert a hexadecimal number to decimal."""

import argparse


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Convert a hexadecimal number to decimal."
    )
    parser.add_argument(
        "hex_value",
        nargs="?",
        help="hexadecimal value to convert (for example: 2A or 0x2A)",
    )
    args = parser.parse_args()
    hex_value = args.hex_value
    if hex_value is None:
        hex_value = input("Enter a hexadecimal number: ").strip()

    try:
        decimal_value = int(hex_value, 16)
    except ValueError:
        parser.error(f"{hex_value!r} is not a valid hexadecimal number")

    print(decimal_value)


if __name__ == "__main__":
    main()
