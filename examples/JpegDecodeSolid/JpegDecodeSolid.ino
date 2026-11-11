/*
 * JpegDecodeSolid - esp_new_jpeg decoder example
 *
 * Decodes an embedded 32x24 JPEG into RGB565 and prints the header info + a few pixels.
 * No SD card / camera needed.
 */
#include "esp_jpeg_dec.h"
#include "jpeg_data.h"

// 解码输出缓冲必须 16 字节对齐
static uint8_t outStorage[32 * 24 * 2] __attribute__((aligned(16)));
static uint16_t *outBuf = (uint16_t *)outStorage;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("esp_new_jpeg decoder example");
  Serial.printf("input JPEG: %u bytes\n", TEST_JPEG_LEN);

  jpeg_dec_config_t cfg = DEFAULT_JPEG_DEC_CONFIG();
  cfg.output_type = JPEG_PIXEL_FORMAT_RGB565_LE;

  jpeg_dec_handle_t dec = NULL;
  if (jpeg_dec_open(&cfg, &dec) != JPEG_ERR_OK) {
    Serial.println("jpeg_dec_open failed");
    return;
  }

  jpeg_dec_io_t io = {};
  jpeg_dec_header_info_t info = {};
  io.inbuf = (uint8_t *)TEST_JPEG;
  io.inbuf_len = TEST_JPEG_LEN;

  jpeg_error_t err = jpeg_dec_parse_header(dec, &io, &info);
  if (err == JPEG_ERR_OK) {
    Serial.printf("header: %u x %u\n", info.width, info.height);
    io.outbuf = outStorage;
    err = jpeg_dec_process(dec, &io);
    if (err == JPEG_ERR_OK) {
      // 打印四个角与中心点，验证颜色/方向（左红右蓝、底部绿）
      const int W = info.width, H = info.height;
      Serial.printf("pixel(2,2)=0x%04X  pixel(%d,2)=0x%04X\n", outBuf[2 * W + 2], W - 3, outBuf[2 * W + (W - 3)]);
      Serial.printf("pixel(2,%d)=0x%04X (expect green)\n", H - 3, outBuf[(H - 3) * W + 2]);
    } else {
      Serial.printf("jpeg_dec_process failed: %d\n", (int)err);
    }
  } else {
    Serial.printf("jpeg_dec_parse_header failed: %d\n", (int)err);
  }
  jpeg_dec_close(dec);
}

void loop() {
  delay(1000);
}