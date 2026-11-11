/*
 * JpegEncodeSolid - esp_new_jpeg encoder example
 *
 * Generates a 320x240 gradient image in RGB565, encodes it to JPEG and prints the size.
 * Requires the prebuilt esp_new_jpeg library (this library).
 */
#include "esp_jpeg_enc.h"

static const int W = 320;
static const int H = 240;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("esp_new_jpeg encoder example");

  uint8_t *in = (uint8_t *)jpeg_calloc_align(W * H * 2, 16);
  uint8_t *out = (uint8_t *)malloc(200 * 1024);
  if (!in || !out) {
    Serial.println("alloc failed (PSRAM recommended)");
    return;
  }

  // RG565 little-endian gradient
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      uint16_t r = (x * 31) / (W - 1);
      uint16_t g = (y * 63) / (H - 1);
      uint16_t b = 15;
      uint16_t v = (uint16_t)((r << 11) | (g << 5) | b);
      size_t i = (size_t)y * W * 2 + x * 2;
      in[i] = (uint8_t)(v & 0xFF);
      in[i + 1] = (uint8_t)(v >> 8);
    }
  }

  jpeg_enc_config_t cfg = DEFAULT_JPEG_ENC_CONFIG();
  cfg.width = W;
  cfg.height = H;
  cfg.src_type = JPEG_PIXEL_FORMAT_RGB565_LE;
  cfg.subsampling = JPEG_SUBSAMPLE_420;
  cfg.quality = 60;

  jpeg_enc_handle_t enc = NULL;
  if (jpeg_enc_open(&cfg, &enc) != JPEG_ERR_OK) {
    Serial.println("jpeg_enc_open failed");
    return;
  }

  int outSize = 0;
  jpeg_error_t err = jpeg_enc_process(enc, in, W * H * 2, out, 200 * 1024, &outSize);
  jpeg_enc_close(enc);

  if (err == JPEG_ERR_OK && outSize > 0) {
    Serial.printf("encoded %dx%d -> %d bytes (%.1f%% of raw)\n", W, H, outSize,
                  100.0f * outSize / (W * H * 2));
  } else {
    Serial.printf("encode failed: %d\n", (int)err);
  }

  jpeg_free_align(in);
  free(out);
}

void loop() {
  delay(1000);
}
