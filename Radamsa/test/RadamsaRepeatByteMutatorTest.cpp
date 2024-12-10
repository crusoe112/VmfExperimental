/* Notes for future me:
* solved missing Radamsa definitions by adding "target_link_directories" in unittest cmakelists, adding .../vmf_install/plugins, then adding entry for radamsa lib to "target_link_libraries"
*/

#include "gtest/gtest.h"
#include "../../../VaderModularFuzzer/test/unittest/ModuleTestHelper.hpp"
#include "SimpleStorage.hpp"
#include "../vmf/src/modules/common/mutator/RadamsaRepeatByteMutator.hpp"

// using namespace vmf;
using vmf::StorageModule;
using vmf::StorageRegistry;
using vmf::ModuleTestHelper;
using vmf::TestConfigInterface;
using vmf::SimpleStorage;
using vmf::StorageEntry;
using vmf::RadamsaRepeatByteMutator;
using vmf::BaseException;

// #define GTEST_COUT std::cerr << "[          ] [ INFO ]"

class RadamsaRepeatByteMutatorTest : public ::testing::Test {
  protected:
    RadamsaRepeatByteMutatorTest() 
    {
      storage = new SimpleStorage("storage");
      registry = new StorageRegistry("TEST_INT", StorageRegistry::INT, StorageRegistry::ASCENDING);
      metadata = new StorageRegistry();
      testHelper = new ModuleTestHelper();
      theMutator = new RadamsaRepeatByteMutator("RadamsaRepeatByteMutator");
      config = testHelper -> getConfig();
    }

    ~RadamsaRepeatByteMutatorTest() override {}

    void SetUp() override {
      testCaseKey = registry->registerKey(
          "TEST_CASE", 
          StorageRegistry::BUFFER, 
          StorageRegistry::READ_WRITE
      );
      // int_key = registry->registerKey(
      //     "TEST_INT",
      //     StorageRegistry::INT,
      //     StorageRegistry::READ_WRITE
      // );
      // normalTag = registry->registerTag(
      //     "RAN_SUCCESSFULLY",
      //     StorageRegistry::WRITE_ONLY
      // );
      // // registry->validateRegistration();
      storage->configure(registry, metadata);
      theMutator->init(*config);
      theMutator->registerStorageNeeds(*registry);
      theMutator->registerMetadataNeeds(*metadata);
  }

    void TearDown() override {
      delete registry;
      delete metadata;
      delete storage;
    }

    RadamsaRepeatByteMutator* theMutator;
    StorageModule* storage;
    StorageRegistry* registry;
    StorageRegistry* metadata;
    ModuleTestHelper* testHelper;
    TestConfigInterface* config;
    int testCaseKey;
};

TEST_F(RadamsaRepeatByteMutatorTest, TestOneByteRepeat)
{
  StorageEntry* baseEntry = storage->createNewEntry();
  StorageEntry* modEntry = storage->createNewEntry();

  char* buff = baseEntry->allocateBuffer(testCaseKey, 1);
  buff[0] = '4';

  try{
      theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
  } 
  catch (BaseException e)
  {
    FAIL() << "Exception thrown: " << e.getReason();
  }

  char* modBuff = modEntry->getBufferPointer(testCaseKey);
  int buff_len = modEntry->getBufferSize(testCaseKey);
  int counter = 0;

  for(int i=0; i < buff_len; i++){
    if(modBuff[i] == '4')
      counter++;
  }

  EXPECT_GT(modEntry->getBufferSize(testCaseKey), baseEntry->getBufferSize(testCaseKey) + 1);
  EXPECT_EQ(buff_len - 1, counter);
}

TEST_F(RadamsaRepeatByteMutatorTest, TestTwoBytesRepeat)
{
  StorageEntry* baseEntry = storage->createNewEntry();
  StorageEntry* modEntry = storage->createNewEntry();

  char* buff = baseEntry->allocateBuffer(testCaseKey, 2);
  buff[0] = '4';
  buff[1] = '5';

  try{
      theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
  } 
  catch (BaseException e)
  {
    FAIL() << "Exception thrown: " << e.getReason();
  }

  char* modBuff = modEntry->getBufferPointer(testCaseKey);
  int buff_len = modEntry->getBufferSize(testCaseKey);
  int counter = 0;

  for(int i=0; i < buff_len; i++){
    if(modBuff[i] == '4')
      counter++;
  }

  EXPECT_GT(modEntry->getBufferSize(testCaseKey), baseEntry->getBufferSize(testCaseKey) + 1);
  EXPECT_TRUE(counter == 1 | counter == buff_len - 2);
}