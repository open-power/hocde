# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: import/tools/imageProcs/xgpe_image.mk $
#
# OpenPOWER EKB Project
#
# COPYRIGHT 2016,2019
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG

# $1 == chipId
#
# FIXME -
# The following define has the full P10 build image sections. However, since
# some of the PPE binaries are not yet getting built, we will use the define
# BUILD_XGPE_IMAGE_TEMP at the end instead as the binaries become incrementally
# available.
#TODO RTC 244611 Auxiliary Task:  pull from EKB and install in hw_image.
define BUILD_XGPE_IMAGE
$(eval IMAGE=$1.xgpe_image)

$(eval $(IMAGE)_PATH=$(IMAGEPATH)/xgpe_image)
$(eval $(IMAGE)_LINK_SCRIPT=xgpe_image.cmd)
$(eval $(IMAGE)_LAYOUT=$(IMAGEPATH)/xgpe_image/xgpe_image.o)
$(eval xgpe_image_COMMONFLAGS += -I$(ROOTPATH)/chips/p10/utils/imageProcs/)

# Files with multiple DD level content to be generated (arg2=empty since
# there is no hw/sim variations)
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$1,,xpmr_hdr))
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$1,,xgpe))

# Files to be appended to image
$(eval $(IMAGE)_FILE_XPMR_HDR=$$($(IMAGE)_DD_CONT_xpmr_hdr))
$(eval $(IMAGE)_FILE_LVL1_BL=$(IMAGEPATH)/xgpe_lvl1_copier/xgpe_lvl1_copier.bin)
$(eval $(IMAGE)_FILE_LVL2_BL=$(IMAGEPATH)/xgpe_lvl2_loader/xgpe_lvl2_loader.bin)
$(eval $(IMAGE)_FILE_HCODE=$$($(IMAGE)_DD_CONT_xgpe))
$(eval $(IMAGE)_FILE_AUX_TASK=$(IMAGEPATH)/xgpe_aux_task/xgpe_aux_task.bin)

# Setup dependencies for
# - building image ( $(IMAGE)_DEPS_IMAGE )
# - appending image sections in sequence ( $(IMAGE)_DEPS_{<section>,REPORT} )
#   - file to be appended
#   - all dependencies of previously appended sections or on raw image
#   - append operation as to other section that has to be finished first
$(eval $(IMAGE)_DEPS_XPMR_HDR =$$($(IMAGE)_FILE_XPMR_HDR))
$(eval $(IMAGE)_DEPS_XPMR_HDR+=$$($(IMAGE)_PATH)/.$(IMAGE).setbuild_head_commit)

$(eval $(IMAGE)_DEPS_LVL1_BL =$$($(IMAGE)_FILE_LVL1_BL))
$(eval $(IMAGE)_DEPS_LVL1_BL+=$$($(IMAGE)_DEPS_XPMR_HDR))
$(eval $(IMAGE)_DEPS_LVL1_BL+=$$($(IMAGE)_PATH)/.$(IMAGE).append.xpmr_hdr)

$(eval $(IMAGE)_DEPS_LVL2_BL =$$($(IMAGE)_FILE_LVL2_BL))
$(eval $(IMAGE)_DEPS_LVL2_BL+=$$($(IMAGE)_DEPS_LVL1_BL))
$(eval $(IMAGE)_DEPS_LVL2_BL+=$$($(IMAGE)_PATH)/.$(IMAGE).append.lvl1_bl)

$(eval $(IMAGE)_DEPS_HCODE =$$($(IMAGE)_FILE_HCODE))
$(eval $(IMAGE)_DEPS_HCODE+=$$($(IMAGE)_DEPS_LVL2_BL))
$(eval $(IMAGE)_DEPS_HCODE+=$$($(IMAGE)_PATH)/.$(IMAGE).append.lvl2_bl)

$(eval $(IMAGE)_DEPS_AUX_TASK =$$($(IMAGE)_FILE_AUX_TASK))
$(eval $(IMAGE)_DEPS_AUX_TASK+=$$($(IMAGE)_DEPS_HCODE))
$(eval $(IMAGE)_DEPS_AUX_TASK+=$$($(IMAGE)_PATH)/.$(IMAGE).append.hcode)

$(eval $(IMAGE)_DEPS_REPORT =$$($(IMAGE)_DEPS_AUX_TASK))
$(eval $(IMAGE)_DEPS_REPORT+=$$($(IMAGE)_PATH)/.$(IMAGE).append.aux_task)

# Image build using all files and serialized by dependencies
$(eval $(call XIP_TOOL,append,.xpmr_hdr,$$($(IMAGE)_DEPS_XPMR_HDR),$$($(IMAGE)_FILE_XPMR_HDR) 1))
$(eval $(call XIP_TOOL,append,.lvl1_bl,$$($(IMAGE)_DEPS_LVL1_BL),$$($(IMAGE)_FILE_LVL1_BL)))
$(eval $(call XIP_TOOL,append,.lvl2_bl,$$($(IMAGE)_DEPS_LVL2_BL),$$($(IMAGE)_FILE_LVL2_BL)))
$(eval $(call XIP_TOOL,append,.hcode,$$($(IMAGE)_DEPS_HCODE), $$($(IMAGE)_FILE_HCODE) 1))
$(eval $(call XIP_TOOL,append,.aux_task,$$($(IMAGE)_DEPS_AUX_TASK),$$($(IMAGE)_FILE_AUX_TASK)))

# Create image report for image with all files appended
$(eval $(call XIP_TOOL,report,,$$($(IMAGE)_DEPS_REPORT)))

$(eval $(call BUILD_XIPIMAGE))
endef

define BUILD_XGPE_IMAGE_TEMP
$(eval IMAGE=$1.xgpe_image)

$(eval $(IMAGE)_PATH=$(IMAGEPATH)/xgpe_image)
$(eval $(IMAGE)_LINK_SCRIPT=xgpe_image.cmd)
$(eval $(IMAGE)_LAYOUT=$(IMAGEPATH)/xgpe_image/xgpe_image.o)
$(eval xgpe_image_COMMONFLAGS += -I$(ROOTPATH)/chips/p10/utils/imageProcs/)

# Files with multiple DD level content to be generated (arg2=empty since
# there is no hw/sim variations)
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$1,,xpmr_hdr))
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$1,,xgpe))

# Files to be appended to image
$(eval $(IMAGE)_FILE_XPMR_HDR=$$($(IMAGE)_DD_CONT_xpmr_hdr))
$(eval $(IMAGE)_FILE_LVL1_BL=$(IMAGEPATH)/xgpe_lvl1_copier/xgpe_lvl1_copier.bin)
$(eval $(IMAGE)_FILE_LVL2_BL=$(IMAGEPATH)/xgpe_lvl2_loader/xgpe_lvl2_loader.bin)
$(eval $(IMAGE)_FILE_HCODE=$$($(IMAGE)_DD_CONT_xgpe))

# Setup dependencies for
# - building image ( $(IMAGE)_DEPS_IMAGE )
# - appending image sections in sequence ( $(IMAGE)_DEPS_{<section>,REPORT} )
#   - file to be appended
#   - all dependencies of previously appended sections or on raw image
#   - append operation as to other section that has to be finished first
$(eval $(IMAGE)_DEPS_IMAGE     = $$($(IMAGE)_FILE_XPMR_HDR))
$(eval $(IMAGE)_DEPS_XPMR_HDR  = $$($(IMAGE)_FILE_XPMR_HDR))
$(eval $(IMAGE)_DEPS_XPMR_HDR += $$($(IMAGE)_PATH)/.$(IMAGE).setbuild_head_commit)

$(eval $(IMAGE)_DEPS_IMAGE    += $$($(IMAGE)_FILE_LVL1_BL))
$(eval $(IMAGE)_DEPS_LVL1_BL   = $$($(IMAGE)_FILE_LVL1_BL))
$(eval $(IMAGE)_DEPS_LVL1_BL  += $$($(IMAGE)_DEPS_XPMR_HDR))
$(eval $(IMAGE)_DEPS_LVL1_BL  += $$($(IMAGE)_PATH)/.$(IMAGE).append.xpmr_hdr)

$(eval $(IMAGE)_DEPS_IMAGE    += $$($(IMAGE)_FILE_LVL2_BL))
$(eval $(IMAGE)_DEPS_LVL2_BL   = $$($(IMAGE)_FILE_LVL2_BL))
$(eval $(IMAGE)_DEPS_LVL2_BL  += $$($(IMAGE)_DEPS_LVL1_BL))
$(eval $(IMAGE)_DEPS_LVL2_BL  += $$($(IMAGE)_PATH)/.$(IMAGE).append.lvl1_bl)

$(eval $(IMAGE)_DEPS_IMAGE    += $$($(IMAGE)_FILE_HCODE))
$(eval $(IMAGE)_DEPS_HCODE     = $$($(IMAGE)_FILE_HCODE))
$(eval $(IMAGE)_DEPS_HCODE    += $$($(IMAGE)_DEPS_LVL2_BL))
$(eval $(IMAGE)_DEPS_HCODE    += $$($(IMAGE)_PATH)/.$(IMAGE).append.lvl2_bl)

$(eval $(IMAGE)_DEPS_REPORT    = $$($(IMAGE)_DEPS_HCODE))
$(eval $(IMAGE)_DEPS_REPORT   += $$($(IMAGE)_PATH)/.$(IMAGE).append.hcode)

# Image build using all files and serialized by dependencies
$(eval $(call XIP_TOOL,append,.xpmr_hdr,$$($(IMAGE)_DEPS_XPMR_HDR),$$($(IMAGE)_FILE_XPMR_HDR) 1))
$(eval $(call XIP_TOOL,append,.lvl1_bl,$$($(IMAGE)_DEPS_LVL1_BL),$$($(IMAGE)_FILE_LVL1_BL)))
$(eval $(call XIP_TOOL,append,.lvl2_bl,$$($(IMAGE)_DEPS_LVL2_BL),$$($(IMAGE)_FILE_LVL2_BL)))
$(eval $(call XIP_TOOL,append,.hcode,$$($(IMAGE)_DEPS_HCODE), $$($(IMAGE)_FILE_HCODE) 1))

# Create image report for image with all files appended
$(eval $(call XIP_TOOL,report,,$$($(IMAGE)_DEPS_REPORT)))

$(eval $(call BUILD_XIPIMAGE,$$($(IMAGE)_DEPS_IMAGE)))
endef

$(eval MYCHIPS := $(filter-out ocmb,$(CHIPS)))

$(foreach chip,$(MYCHIPS),\
	$(foreach chipId, $($(chip)_CHIPID),\
		$(eval $(call BUILD_XGPE_IMAGE_TEMP,$(chipId)))))
