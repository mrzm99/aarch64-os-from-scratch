#/*------------------------------------------------------*/
#/*!
# *      @file   main.c
# *      @date   2026.xx.xx
# *      @author mrzm99
# *      @brief
# *      @note
# */
#/*------------------------------------------------------*/

# ツールチェーン
PREFIX = aarch64-linux-gnu-
CC = $(PREFIX)gcc
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy

# ビルド生成物用ディレクトリ設定
BUILD_DIR = build

# コンパイルオプション
# -ffreestanding: 標準ライブラリ(printfなど)が存在しない
# -mgeneral-regs-only: 浮動小数点レジスタを使用しない
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles -mgeneral-regs-only
LDFLAGS = -T scripts/linker_script.ld

# ソースファイル
ASM_SRCS = \
		   arch/aarch64/boot.S \
		   arch/aarch64/exception.S \
		   arch/aarch64/mmu.S

C_SRCS = \
		 kernel/main.c \
		 arch/aarch64/uart.c \
		 kernel/printk.c \
		 kernel/exception.c \
		 kernel/mm/pmm.c \
		 kernel/mm/vmm.c

# オブジェクトファイル名の生成
ASM_OBJS = $(patsubst %.S, $(BUILD_DIR)/%.o, $(ASM_SRCS))
C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SRCS))

# オブジェクトファイル
OBJS = \
	   $(ASM_OBJS) \
	   $(C_OBJS)

# デフォルトターゲット
all: $(BUILD_DIR)/os.elf $(BUILD_DIR)/os.bin

# ELFファイルの生成
$(BUILD_DIR)/os.elf: $(OBJS) scripts/linker_script.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# ベタ書きバイナリの生成
$(BUILD_DIR)/os.bin: $(BUILD_DIR)/os.elf
	$(OBJCOPY) -O binary $< $@

# アセンブラのコンパイル
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Cファイルのコンパイル
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# クリーン
clean:
	rm -rf $(BUILD_DIR)
