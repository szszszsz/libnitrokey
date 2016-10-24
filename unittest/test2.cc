#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()
#include "catch.hpp"

#include <iostream>
#include <string.h>
#include <NitrokeyManager.h>
#include "device_proto.h"
#include "log.h"
//#include "stick10_commands.h"
#include "stick20_commands.h"

using namespace std;
using namespace nitrokey::device;
using namespace nitrokey::proto;
using namespace nitrokey::proto::stick20;
using namespace nitrokey::log;
using namespace nitrokey::misc;


template<typename CMDTYPE>
void execute_password_command(Device &stick, const char *password, const char kind = 'P') {
  auto p = get_payload<CMDTYPE>();
  if (kind == 'P'){
    p.set_kind_user();
  } else {
    p.set_kind_admin();
  }
  strcpyT(p.password, password);
  CMDTYPE::CommandTransaction::run(stick, p);
}


TEST_CASE("test", "[test]") {
  Stick20 stick;
  bool connected = stick.connect();
  REQUIRE(connected == true);

  Log::instance().set_loglevel(Loglevel::DEBUG_L2);

  stick10::LockDevice::CommandTransaction::run(stick);
//  execute_password_command<EnableEncryptedPartition>(stick, "123456");
//  execute_password_command<DisableEncryptedPartition>(stick, "123456");
  this_thread::sleep_for(1000ms);
  execute_password_command<EnableEncryptedPartition>(stick, "123456");
  this_thread::sleep_for(1000ms);
  this_thread::sleep_for(1000ms);
  this_thread::sleep_for(1000ms);
  this_thread::sleep_for(1000ms);
  execute_password_command<EnableHiddenEncryptedPartition>(stick, "123123123");
  this_thread::sleep_for(1000ms);
  execute_password_command<DisableHiddenEncryptedPartition>(stick, "123123123");
  this_thread::sleep_for(1000ms);
  execute_password_command<SendSetReadonlyToUncryptedVolume>(stick, "123456");
  this_thread::sleep_for(1000ms);
  execute_password_command<SendSetReadwriteToUncryptedVolume>(stick, "123456");
  this_thread::sleep_for(1000ms);
  execute_password_command<SendClearNewSdCardFound>(stick, "12345678", 'A');
  this_thread::sleep_for(1000ms);
//  execute_password_command<LockFirmware>(stick, "123123123");
//  execute_password_command<EnableFirmwareUpdate>(stick, "123123123"); //FIRMWARE PIN

  stick10::LockDevice::CommandTransaction::run(stick);

  stick.disconnect();
}
