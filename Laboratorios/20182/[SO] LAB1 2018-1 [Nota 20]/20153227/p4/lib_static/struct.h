#define V2_NR_TZONES  10

typedef unsigned long  ino_t;
typedef unsigned short zone1_t;
typedef long           off_t;
typedef unsigned long  zone_t;
typedef unsigned long  bit_t;
typedef long           time_t;
typedef unsigned long  ino_t; 

typedef struct {
  ino_t s_ninodes;      /* # usable inodes on the minor device */
  zone1_t s_nzones;     /* total device size, including bit maps etc */
  short s_imap_blocks; /* # of blocks used by inode bit map */
  short s_zmap_blocks; /* # of blocks used by zone bit map */
  zone1_t s_firstdatazone;    /* number of first data zone */
  short s_log_zone_size;      /* log2 of blocks/zone */
  short s_pad;          /* try to avoid compiler-dependent padding */
  off_t s_max_size;     /* maximum file size on this device */
  zone_t s_zones;       /* number of zones (replaces s_nzones in V2) */
  short s_magic;        /* magic number to recognize super-blocks */
  /* The following items are valid on disk only for V3 and above */
  /* The block size in bytes. Minimum MIN_BLOCK SIZE. SECTOR_SIZE
   * multiple. If V1 or V2 filesystem, this should be
   * initialised to STATIC_BLOCK_SIZE. Maximum MAX_BLOCK_SIZE.
   */
  short s_pad2;         /* try to avoid compiler-dependent padding */
  unsigned short s_block_size;      /* block size in bytes. */
  char s_disk_version; /* filesystem format sub-version */
} super_block;

typedef struct {
  short i_mode;         /* file type, protection, etc. */
  short i_nlinks;       /* how many links to this file */
  short i_uid;          /* user id of the file's owner */
  short i_gid;          /* group number */
  off_t i_size;         /* current file size in bytes */
  time_t i_atime;       /* time of last access (V2 only) */
  time_t i_mtime;       /* when was file data last changed */
  time_t i_ctime;       /* when was inode itself changed (V2 only)*/
  zone_t i_zone[V2_NR_TZONES];/*zone numbers for direct,ind,and dbl ind */
} inode;

typedef struct {
      ino_t d_inodo;
      char d_name[60];
} entry_dir;











