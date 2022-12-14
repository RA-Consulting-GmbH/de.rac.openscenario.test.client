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


#include "TestFilesV1_2.h"

bool TestV1_2(std::string basePath)
{
	NET_ASAM_OPENSCENARIO::v1_2::TestFiles testFiles(basePath);
	

	auto result = true;

	
	result = testFiles.TestSimpleSuccess() && result;
	
	return result;
}



int main(int argc, char** argv)
{
#if defined(_DEBUG) && defined(WIN32)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string basePath = ".";
  	
  auto result = true;
  result = TestV1_2(basePath) && result;

  if (result)
    return 0;

  return -1;
}
