/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT OutFile
{
public:
	explicit OutFile(const QString &fileName);
	~OutFile();
	QTextStream &operator()();

	/// Flushes all new modifications in text stream into the file system.
	void flush();

	/// Creates in heap new OutFile instance if it can be done
	/// or returns nullptr and writes into log error message otherwise.
	static OutFile *openOrLogError(const QString &fileName);

private:
	QTextStream mOut;
	QFile mFile;
};

}
