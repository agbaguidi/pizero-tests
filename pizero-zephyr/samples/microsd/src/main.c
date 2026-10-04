/*
 * Copyright (c) 2026 Nono
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Onboard microSD (SPI SDHC + FatFS) bring-up sample.
 * Card detect is software-only: disk init / mount success means present.
 */

#include <stdio.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/sys/printk.h>

#include <ff.h>

LOG_MODULE_REGISTER(microsd, LOG_LEVEL_INF);

#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/" DISK_DRIVE_NAME ":"
#define SAMPLE_PATH DISK_MOUNT_PT "/pizero.txt"
#define SAMPLE_TEXT "pizero-zephyr microsd ok\n"

static FATFS fat_fs;

static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.fs_data = &fat_fs,
	.mnt_point = DISK_MOUNT_PT,
};

static int probe_disk(void)
{
	uint32_t block_count = 0;
	uint32_t block_size = 0;
	uint64_t size_mb;
	int rc;

	rc = disk_access_ioctl(DISK_DRIVE_NAME, DISK_IOCTL_CTRL_INIT, NULL);
	if (rc != 0) {
		printk("No card detected (disk init failed: %d)\n", rc);
		return rc;
	}

	rc = disk_access_ioctl(DISK_DRIVE_NAME, DISK_IOCTL_GET_SECTOR_COUNT,
			       &block_count);
	if (rc != 0) {
		printk("Unable to get sector count (%d)\n", rc);
		goto out;
	}

	rc = disk_access_ioctl(DISK_DRIVE_NAME, DISK_IOCTL_GET_SECTOR_SIZE,
			       &block_size);
	if (rc != 0) {
		printk("Unable to get sector size (%d)\n", rc);
		goto out;
	}

	size_mb = ((uint64_t)block_count * block_size) >> 20;
	printk("Card present: %u sectors x %u bytes (~%u MB)\n", block_count,
	       block_size, (uint32_t)size_mb);

out:
	(void)disk_access_ioctl(DISK_DRIVE_NAME, DISK_IOCTL_CTRL_DEINIT, NULL);
	return rc;
}

static int lsdir(const char *path)
{
	struct fs_dir_t dir;
	struct fs_dirent entry;
	int rc;
	int count = 0;

	fs_dir_t_init(&dir);

	rc = fs_opendir(&dir, path);
	if (rc != 0) {
		printk("opendir %s failed (%d)\n", path, rc);
		return rc;
	}

	printk("Listing %s\n", path);
	while (true) {
		rc = fs_readdir(&dir, &entry);
		if (rc != 0 || entry.name[0] == '\0') {
			break;
		}

		printk("  %s %s (%zu)\n",
		       entry.type == FS_DIR_ENTRY_DIR ? "DIR " : "FILE",
		       entry.name, entry.size);
		count++;
	}

	(void)fs_closedir(&dir);
	printk("%d entries\n", count);
	return 0;
}

static int write_sample_file(void)
{
	struct fs_file_t file;
	ssize_t written;
	int rc;

	fs_file_t_init(&file);

	rc = fs_open(&file, SAMPLE_PATH, FS_O_CREATE | FS_O_WRITE | FS_O_TRUNC);
	if (rc != 0) {
		printk("open %s for write failed (%d)\n", SAMPLE_PATH, rc);
		return rc;
	}

	written = fs_write(&file, SAMPLE_TEXT, strlen(SAMPLE_TEXT));
	(void)fs_close(&file);

	if (written < 0) {
		printk("write failed (%d)\n", (int)written);
		return (int)written;
	}

	printk("Wrote %d bytes to %s\n", (int)written, SAMPLE_PATH);
	return 0;
}

static int read_sample_file(void)
{
	struct fs_file_t file;
	char buf[64];
	ssize_t n;
	int rc;

	fs_file_t_init(&file);

	rc = fs_open(&file, SAMPLE_PATH, FS_O_READ);
	if (rc != 0) {
		printk("open %s for read failed (%d)\n", SAMPLE_PATH, rc);
		return rc;
	}

	n = fs_read(&file, buf, sizeof(buf) - 1);
	(void)fs_close(&file);

	if (n < 0) {
		printk("read failed (%d)\n", (int)n);
		return (int)n;
	}

	buf[n] = '\0';
	printk("Read back (%d bytes): %s", (int)n, buf);
	return 0;
}

int main(void)
{
	int rc;

	printk("microSD FatFS sample\n");

	if (!device_is_ready(DEVICE_DT_GET(DT_NODELABEL(sdhc0)))) {
		printk("ERROR: sdhc0 is not ready\n");
		return 0;
	}

	rc = probe_disk();
	if (rc != 0) {
		printk("Insert a FAT32-formatted microSD and reset.\n");
		return 0;
	}

	rc = fs_mount(&mp);
	if (rc != FR_OK) {
		printk("Mount failed (%d). Format the card as FAT32.\n", rc);
		return 0;
	}

	printk("Mounted %s\n", DISK_MOUNT_PT);

	(void)lsdir(DISK_MOUNT_PT);
	(void)write_sample_file();
	(void)read_sample_file();
	(void)lsdir(DISK_MOUNT_PT);

	rc = fs_unmount(&mp);
	if (rc != FR_OK) {
		printk("Unmount failed (%d)\n", rc);
	} else {
		printk("Unmounted\n");
	}

	printk("Done.\n");
	return 0;
}
