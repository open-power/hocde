# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: import/tools/imageProcs/restore_image.mk $
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
IMAGE=restore_image
$(call ADD_IMAGE_INCDIR,$(IMAGE),$(ROOTPATH)/chips/p9/xip)
#$(call XIP_TOOL,append,.cpmr,$(ROOTPATH)/chips/p9/procedures/ppe_closed/cme/stop_cme/obj/stop_cme/cpmr_header.bin)
$(call XIP_TOOL,append,.self_restore,$(ROOTPATH)/chips/p9/procedures/utils/stopreg/selfRest.bin)
$(call BUILD_IMAGE)
