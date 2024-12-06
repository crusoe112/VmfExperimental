/* Notes for future me:
* solved missing Radamsa definitions by adding "target_link_directories" in unittest cmakelists, adding .../vmf_install/plugins, then adding entry for radamsa lib to "target_link_libraries"
*/

#include "gtest/gtest.h"
#include "../../../VaderModularFuzzer/test/unittest/ModuleTestHelper.hpp"
#include "SimpleStorage.hpp"
#include "../vmf/src/modules/common/mutator/RadamsaIncrementByteMutator.hpp"

// using namespace vmf;
using vmf::StorageModule;
using vmf::StorageRegistry;
using vmf::ModuleTestHelper;
using vmf::TestConfigInterface;
using vmf::SimpleStorage;
using vmf::StorageEntry;
using vmf::RadamsaIncrementByteMutator;
using vmf::BaseException;

// #define GTEST_COUT std::cerr << "[          ] [ INFO ]"

class RadamsaIncrementByteMutatorTest : public ::testing::Test {
  protected:
    RadamsaIncrementByteMutatorTest() 
    {
      storage = new SimpleStorage("storage");
      registry = new StorageRegistry("TEST_INT", StorageRegistry::INT, StorageRegistry::ASCENDING);
      metadata = new StorageRegistry();
      testHelper = new ModuleTestHelper();
      theMutator = new RadamsaIncrementByteMutator("RadamsaIncrementByteMutator");
      config = testHelper -> getConfig();
    }

    ~RadamsaIncrementByteMutatorTest() override {}

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

    RadamsaIncrementByteMutator* theMutator;
    StorageModule* storage;
    StorageRegistry* registry;
    StorageRegistry* metadata;
    ModuleTestHelper* testHelper;
    TestConfigInterface* config;
    int testCaseKey;
};

TEST_F(RadamsaIncrementByteMutatorTest, TestBufferSize)
{
  // Test that the mutated entry is one byte larger than the base entry

  StorageEntry* baseEntry = storage->createNewEntry();
  StorageEntry* modEntry = storage->createNewEntry();

  char* buff = baseEntry->allocateBuffer(testCaseKey, 12);
  buff[0] = 'T';
  buff[1] = 'E';
  buff[2] = 'S';
  buff[3] = 'T';
  buff[4] = '1';
  buff[5] = '2';
  buff[6] = '3';
  buff[7] = '4';
  buff[8] = '5';
  buff[9] = '6';
  buff[10] = '7';
  buff[11] = '8';

  try{
      theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
  } 
  catch (BaseException e)
  {
    FAIL() << "Exception thrown: " << e.getReason();
  }

  EXPECT_EQ(baseEntry->getBufferSize(testCaseKey) + 1, modEntry->getBufferSize(testCaseKey));
}

TEST_F(RadamsaIncrementByteMutatorTest, TestByteIncremented)
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
  EXPECT_EQ(modBuff[0], '5');
}
