# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

QT += widgets network

includes(plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/common/trikKit \
		plugins/robots/common \
		plugins/robots/interpreters \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpreters/trikKitInterpreterCommon \
		plugins/robots/interpreters/interpreterCore \
		plugins/robots/dataFlowInterpreters \
		plugins/robots/dataFlowInterpreters\DFInterpreter \
		)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-trik-kit qextserialport qslog \
		robots-trik-kit-interpreter-common dataflow-interpreter \
		)

HEADERS += \
	$$PWD/DFTrikV62KitInterpreterPlugin.h \

SOURCES += \
	$$PWD/DFTrikV62KitInterpreterPlugin.cpp \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/DFTrikV62KitInterpreter_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/DFTrikV62KitInterpreter_fr.ts \