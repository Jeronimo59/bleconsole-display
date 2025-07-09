
#include "sddrive.h"
#include "ui.h"

// Dummy implementation of sd_ready_cb to fix the undefined identifier error
extern "C" bool sd_ready_cb(struct _lv_fs_drv_t *drv)
{
   Serial.printf("[sddrive] sd_ready_cb(): SD card size is %llu bytes\n", SD.cardSize());
   // Return true to indicate the drive is ready
   for (File file = SD.open("/assets"); file != NULL; file = file.openNextFile())
   {
      if (file.isDirectory())
      {
         Serial.printf("[sddrive] sd_ready_cb(): Directory: %s\n", file.name());
      }
      else
      {
         Serial.printf("[sddrive] sd_ready_cb(): File: %s, Size: %lud bytes\n", file.name(), file.size());
      }
   }
   return true;
}

// Dummy implementation of sd_open_cb to fix the undefined identifier error
extern "C" void *sd_open_cb(struct _lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
   if (!SD.exists(path))
   {
#if LV_USE_LOG
      Serial.printf("[sddrive] sd_open_cb(): file '%s' not found\n", path);
#endif
      return nullptr; // Return nullptr if the file does not exist
   }

// If the file exists, print a message
#if LV_USE_LOG
   Serial.printf("[sddrive] sd_open_cb(): File '%s' found\n", path);
#endif
   File *file = new File();

   if (mode == LV_FS_MODE_WR)
   {
      *file = SD.open(path, FILE_WRITE);
   }
   else if (mode == LV_FS_MODE_RD)
   {
      *file = SD.open(path, FILE_READ);
   }

   if (!file)
   {
#if LV_USE_LOG
      Serial.printf("[sddrive] sd_open_cb(): failed to open file '%s'\n", path);
#endif
      delete file;
      return NULL; // Return NULL if the file couldn't be opened
   }

   return file; // Return the file pointer
}

// Dummy implementation of sd_close_cb to fix the undefined identifier error
extern "C" lv_fs_res_t sd_close_cb(struct _lv_fs_drv_t *drv, void *file_p)
{
   if (file_p != nullptr)
   {
      File *file = static_cast<File *>(file_p);
      if (file)
      {
         file->close(); // Close the file
         delete file;   // Delete the file object
      }
   }
   return LV_FS_RES_OK;
}

// Dummy implementation of sd_read_cb to fix the undefined identifier error
extern "C" lv_fs_res_t sd_read_cb(struct _lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
   // Set bytes read to 0 and return LV_FS_RES_OK as a dummy implementation
   if (br)
      *br = 0;
   return LV_FS_RES_OK;
}

// Dummy implementation of sd_write_cb to fix the undefined identifier error
extern "C" lv_fs_res_t sd_write_cb(struct _lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw)
{
   // Set bytes written to 0 and return LV_FS_RES_OK as a dummy implementation
   if (bw)
      *bw = 0;
   return LV_FS_RES_OK;
}

// Dummy implementation of sd_seek_cb to fix the undefined identifier error
extern "C" lv_fs_res_t sd_seek_cb(struct _lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
   // Return LV_FS_RES_OK as a dummy implementation
   return LV_FS_RES_OK;
}

// Dummy implementation of sd_tell_cb to fix the undefined identifier error
extern "C" lv_fs_res_t sd_tell_cb(struct _lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
   // Set position to 0 and return LV_FS_RES_OK as a dummy implementation
   if (pos_p)
      *pos_p = 0;
   return LV_FS_RES_OK;
}

bool SDDrive::begin(char letter, uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs)
{
   if (isInitialized)
      return true;

   // Initialize the SPI bus

   spi->begin(
       clk,
       miso,
       mosi,
       cs);

   if (!SD.begin(cs, *spi))
      return false;

   Serial.printf("SD card initialized successfully. Card type: %s\n", SD.cardType() == CARD_NONE ? "None" : SD.cardType() == CARD_MMC ? "MMC"
                                                                                                        : SD.cardType() == CARD_SD    ? "SD"
                                                                                                        : SD.cardType() == CARD_SDHC  ? "SDHC"
                                                                                                                                      : "Unknown");

   Serial.printf("\nSD card size: %llu bytes\n", SD.cardSize());

   lv_fs_drv_t drv;
   lv_fs_drv_init(&drv);

   drv.letter = letter;
   drv.cache_size = 0;
   drv.ready_cb = sd_ready_cb;
   drv.open_cb = sd_open_cb;
   drv.close_cb = sd_close_cb;
   drv.read_cb = sd_read_cb;
   drv.write_cb = sd_write_cb;
   drv.seek_cb = sd_seek_cb;
   drv.tell_cb = sd_tell_cb;
   drv.dir_open_cb = nullptr;
   drv.dir_read_cb = nullptr;
   drv.dir_close_cb = nullptr;

   lv_fs_drv_register(&drv);

   lv_fs_drv_t *drv2 = lv_fs_get_drv('S');
   if (drv2 == NULL)
   {
      Serial.printf("Failed to get SD drive '%c'\n", 'S');
      return false;
   }
   if (drv2->open_cb == nullptr)
   {
      Serial.printf("SD drive '%c' open callback is null\n", 'S');
      return false;
   }
   else
   {
      Serial.printf("SD drive '%c' has open() callback\n", 'S');
      lv_image_set_src(ui_ConsoleScreen_Image1, "S:/assets/tgv.bmp");
      if (ui_VehicleScreen_Image)
         lv_image_set_src(ui_VehicleScreen_Image, "S:/assets/tgv.bmp");
      else
         Serial.println("ui_VehicleScreen_Image is NULL, cannot set image source.");
   }
   return isInitialized = true;
}
