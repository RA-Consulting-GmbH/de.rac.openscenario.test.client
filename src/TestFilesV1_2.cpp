/*
 * Copyright 2020 RA Consulting
 *
 * RA Consulting GmbH licenses this file under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TestBaseV1_2.h"
#include "TestFilesV1_2.h"

#include <iostream>
#include "ScenarioLoaderException.h"

namespace NET_ASAM_OPENSCENARIO
{
	namespace v1_2
	{
		TestFiles::TestFiles(std::string& executablePath) : TestBase(executablePath) {}

		double TestFiles::epsilon = 0.001;

		bool TestFiles::TestSimpleSuccess()
		{
			try
			{
				ClearMessageLogger();
				(void)ExecuteParsing(_executablePath + "/" + kInputDir + "DoubleLaneChanger1_2.xosc");
				return Assert(_messageLogger->GetMessagesFilteredByWorseOrEqualToErrorLevel(NET_ASAM_OPENSCENARIO::ERROR).empty(), ASSERT_LOCATION);;
			}
			catch (NET_ASAM_OPENSCENARIO::ScenarioLoaderException& e)
			{
				std::cout << e.what() << std::endl;
				return Assert(false, ASSERT_LOCATION);
			}
		}
	}
}