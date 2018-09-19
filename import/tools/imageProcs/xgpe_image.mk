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

# $1 == type
# $2 == chipId
define BUILD_XGPE_IMAGE
$(eval IMAGE=$2.$1.xgpe_image)

$(eval $(IMAGE)_PATH=$(IMAGEPATH)/xgpe_image)
$(eval $(IMAGE)_LINK_SCRIPT=xgpe_image.cmd)
$(eval $(IMAGE)_LAYOUT=$(IMAGEPATH)/xgpe_image/xgpe_image.o)
$(eval xgpe_image_COMMONFLAGS += -I$(ROOTPATH)/chips/p10/utils/imageProcs/)

# Files with multiple DD level content to be generated
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$2,xpmr_hdr))
$(eval $(call BUILD_DD_LEVEL_CONTAINER,$2,xgpe))

# Files to be appended to image
$(eval $(IMAGE)_FILE_XPMR_HDR=$$($(IMAGE)_DD_CONT_xpmr_hdr))
$(eval $(IMAGE)_FILE_LVL1_BL=$(IMAGEPATH)/xgpe_lvl1_copier/xgpe_lvl1_copier.bin)
$(eval $(IMAGE)_FILE_LVL2_BL=$(IMAGEPATH)/xgpe_lvl2_loader/xgpe_lvl2_loader.bin)
$(eval $(IMAGE)_FILE_HCODE=$$($(IMAGE)_DD_CONT_xgpe))
$(eval $(IMAGE)_FILE_AUX_TASK=$(IMAGEPATH)/xgpe_aux_task/xgpe_aux_task.bin)

# Dependencies for appending image sections in sequence:
# - file to be appended
# - all dependencies of previously appended sections or on raw image
# - append operation as to other section that has to be finished first
$(eval $(IMAGE)_DEPS_XPMR_HDR =$$($(IMAGE)_FILE_XPMR_HDR))
$(eval $(IMAGE)_DEPS_XPMR_HDR+=$$($(IMAGE)_PATH)/.$(IMAGE).setbuild_host)

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

$(eval MYCHIPS := $(filter-out ocmb,$(CHIPS)))

$(foreach chip,$(MYCHIPS),\
	$(foreach chipId, $($(chip)_CHIPID),\
		$(foreach type, $(HW_IMAGE_VARIATIONS),\
			$(eval $(call BUILD_XGPE_IMAGE,$(type),$(chipId))))))
