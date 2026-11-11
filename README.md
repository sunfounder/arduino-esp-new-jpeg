# esp_new_jpeg (Arduino packaging)

Arduino library packaging of the **Espressif `esp_new_jpeg` component v1.0.2** — JPEG **encoder and decoder** for ESP32 / ESP32-S3.

> This is a **repackaging** project: the headers and the prebuilt static libraries come
> **unmodified** from the upstream Espressif component. This repository only adds
> Arduino library metadata, documentation and examples. See `SOURCE.txt` for the
> upstream link and the SHA-256 of the binaries.

## Upstream

| Item | Value |
| --- | --- |
| Component | `esp_new_jpeg` |
| Version | 1.0.2 |
| Vendor | Espressif Systems |
| Source | https://components.espressif.com/components/espressif/esp_new_jpeg |
| Mirror (binary verified) | https://github.com/mikeysklar/esp_new_jpeg (trimmed mirror, 1.0.2) |
| License | **Espressif MIT License** (see `LICENSE`) |

## What is inside

```
src/esp_jpeg_dec.h        JPEG decoder API
src/esp_jpeg_enc.h        JPEG encoder API
src/esp_jpeg_common.h     shared types / pixel formats
src/esp_jpeg_version.h    version macros
src/esp32/libesp_new_jpeg.a    prebuilt, ESP32
src/esp32s3/libesp_new_jpeg.a  prebuilt, ESP32-S3   <-- used by Arduino Nano ESP32
```

## Installation

**Arduino Library Manager** (after this library is accepted into the registry):
search for `esp_new_jpeg` and install.

**Manual / offline**: download the release `.zip`, then
Arduino IDE → *Sketch → Include Library → Add .ZIP Library…*

## Usage

Decode a JPEG buffer to RGB565:

```cpp
#include "esp_jpeg_dec.h"

jpeg_dec_config_t cfg = DEFAULT_JPEG_DEC_CONFIG();
cfg.output_type = JPEG_PIXEL_FORMAT_RGB565_LE;
jpeg_dec_handle_t dec = NULL;
if (jpeg_dec_open(&cfg, &dec) == JPEG_ERR_OK) {
  jpeg_dec_io_t io = {};
  jpeg_dec_header_info_t info = {};
  io.inbuf = jpeg_data;  io.inbuf_len = jpeg_len;
  if (jpeg_dec_parse_header(dec, &io, &info) == JPEG_ERR_OK) {
    io.outbuf = out_rgb565;
    jpeg_dec_process(dec, &io);
  }
  jpeg_dec_close(dec);
}
```

Encode RGB565 to JPEG (input buffer must be 16-byte aligned, see `jpeg_calloc_align`):

```cpp
#include "esp_jpeg_enc.h"

jpeg_enc_config_t ec = DEFAULT_JPEG_ENC_CONFIG();
ec.width = 320; ec.height = 240;
ec.src_type = JPEG_PIXEL_FORMAT_RGB565_LE;
ec.subsampling = JPEG_SUBSAMPLE_420;
ec.quality = 60;
jpeg_enc_handle_t enc = NULL;
if (jpeg_enc_open(&ec, &enc) == JPEG_ERR_OK) {
  int out_size = 0;
  jpeg_enc_process(enc, in_buf, 320 * 240 * 2, out_buf, out_buf_size, &out_size);
  jpeg_enc_close(enc);
}
```

## Examples

- `examples/JpegEncodeSolid` — encodes a generated 320×240 image and prints the JPEG size.

## Notes and limitations

- This package ships the **ESP32-S3** binary only (`src/esp32s3`), verified byte-identical to the upstream 1.0.2 build.
  Classic ESP32 is not included.
- Built against the ESP-IDF version shipped with **Arduino ESP32 core 2.0.18**;
  binary compatibility with newer cores is not guaranteed — verify before relying on it.
- ESP32-P4 / ESP32-C6 etc. are not included (upstream component may support them).