#include "gtest/gtest.h"
#include "../../../VaderModularFuzzer/test/unittest/ModuleTestHelper.hpp"
#include "SimpleStorage.hpp"
#include "../vmf/src/modules/common/mutator/RadamsaFlipByteMutator.hpp"

// using namespace vmf;
using vmf::StorageModule;
using vmf::StorageRegistry;
using vmf::ModuleTestHelper;
using vmf::TestConfigInterface;
using vmf::SimpleStorage;
using vmf::StorageEntry;
using vmf::RadamsaFlipByteMutator;
using vmf::BaseException;

// #define GTEST_COUT std::cerr << "[          ] [ INFO ]"

class RadamsaFlipByteMutatorTest : public ::testing::Test {
  protected:
    RadamsaFlipByteMutatorTest() 
    {
      storage = new SimpleStorage("storage");
      registry = new StorageRegistry("TEST_INT", StorageRegistry::INT, StorageRegistry::ASCENDING);
      metadata = new StorageRegistry();
      testHelper = new ModuleTestHelper();
      theMutator = new RadamsaFlipByteMutator("RadamsaFlipByteMutator");
      config = testHelper -> getConfig();
    }

    ~RadamsaFlipByteMutatorTest() override {}

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

    RadamsaFlipByteMutator* theMutator;
    StorageModule* storage;
    StorageRegistry* registry;
    StorageRegistry* metadata;
    ModuleTestHelper* testHelper;
    TestConfigInterface* config;
    int testCaseKey;
};

TEST_F(RadamsaFlipByteMutatorTest, TestBufferSize)
{
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

  EXPECT_EQ(baseEntry->getBufferSize(testCaseKey)+1, modEntry->getBufferSize(testCaseKey));
}

TEST_F(RadamsaFlipByteMutatorTest, TestByteFlipped)
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
  char expected = buff[0] ^ 0b10; // 0b10 is the mask when using the default seed
  EXPECT_EQ(modBuff[0], expected);
}
