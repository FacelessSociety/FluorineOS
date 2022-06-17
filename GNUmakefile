ARCH ?= x86_64

# Check that the architecture is supported and set relevant variables.
ifeq ($(ARCH),x86_64)
    override EFI_ARCH := X64
else
    $(error Architecture $(ARCH) not supported)
endif

.PHONY: all
all: Fluorine.iso

.PHONY: all-hdd
all-hdd: Fluorine.hdd

.PHONY: run
run: Fluorine.iso
	qemu-system-$(ARCH) -M q35 -m 2G -cdrom Fluorine.iso -boot d -monitor stdio

.PHONY: run-uefi
run-uefi: ovmf-$(EFI_ARCH) Fluorine.iso
	qemu-system-$(ARCH) -M q35 -m 2G -bios ovmf-$(EFI_ARCH)/OVMF.fd -cdrom Fluorine.iso -boot d

.PHONY: run-hdd
run-hdd: Fluorine.hdd
	qemu-system-$(ARCH) -M q35 -m 2G -hda Fluorine.hdd

.PHONY: run-hdd-uefi
run-hdd-uefi: ovmf-$(EFI_ARCH) Fluorine.hdd
	qemu-system-$(ARCH) -M q35 -m 2G -bios ovmf-$(EFI_ARCH)/OVMF.fd -hda Fluorine.hdd

ovmf-$(EFI_ARCH):
	mkdir -p ovmf-$(EFI_ARCH)
	cd ovmf-$(EFI_ARCH) && curl -o OVMF-$(EFI_ARCH).zip https://efi.akeo.ie/OVMF/OVMF-$(EFI_ARCH).zip && 7z x OVMF-$(EFI_ARCH).zip

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1
	make -C limine

.PHONY: kernel
kernel:
	cd kernel; bash buildall

Fluorine.iso: limine kernel
	rm -rf iso_root
	mkdir -p iso_root
	cp kernel/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o Fluorine.iso
	limine/limine-deploy Fluorine.iso
	rm -rf iso_root
	bash kernel/clean

Fluorine.hdd: limine kernel
	rm -f Fluorine.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=Fluorine.hdd
	parted -s Fluorine.hdd mklabel gpt
	parted -s Fluorine.hdd mkpart ESP fat32 2048s 100%
	parted -s Fluorine.hdd set 1 esp on
	limine/limine-deploy Fluorine.hdd
	sudo losetup -Pf --show Fluorine.hdd >loopback_dev
	sudo mkfs.fat -F 32 `cat loopback_dev`p1
	mkdir -p img_mount
	sudo mount `cat loopback_dev`p1 img_mount
	sudo mkdir -p img_mount/EFI/BOOT
	sudo cp -v kernel/kernel.elf limine.cfg limine/limine.sys img_mount/
	sudo cp -v limine/BOOT$(EFI_ARCH).EFI img_mount/EFI/BOOT/
	sync
	sudo umount img_mount
	sudo losetup -d `cat loopback_dev`
	rm -rf loopback_dev img_mount

.PHONY: clean
clean:
	rm -rf iso_root Fluorine.iso Fluorine.hdd
	bash kernel/clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-$(EFI_ARCH)
	$(MAKE) -C kernel distclean
