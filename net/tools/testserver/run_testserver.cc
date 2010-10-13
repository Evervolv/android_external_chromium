// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop.h"
#include "net/test/test_server.h"

static void PrintUsage() {
  printf("run_testserver --doc-root=relpath [--http|--https|--ftp]\n");
  printf("(NOTE: relpath should be relative to the 'src' directory)\n");
}

int main(int argc, const char* argv[]) {
  base::AtExitManager at_exit_manager;
  MessageLoopForIO message_loop;

  // Process command line
  CommandLine::Init(argc, argv);
  CommandLine* command_line = CommandLine::ForCurrentProcess();

  if (command_line->GetSwitchCount() == 0 ||
      command_line->HasSwitch("help")) {
    PrintUsage();
    return -1;
  }

  FilePath doc_root =  command_line->GetSwitchValuePath("doc-root");
  if (doc_root.empty()) {
    printf("Error: --doc-root must be specified\n");
    PrintUsage();
    return -1;
  }

  net::TestServer::Type server_type(net::TestServer::TYPE_HTTP);
  if (command_line->HasSwitch("https")) {
    server_type = net::TestServer::TYPE_HTTPS;
  } else if (command_line->HasSwitch("ftp")) {
    server_type = net::TestServer::TYPE_FTP;
  }

  net::TestServer test_server(server_type, doc_root);
  if (!test_server.Start()) {
    printf("Error: failed to start test server. Exiting.\n");
    return -1;
  }

  printf("testserver running at %s (type ctrl+c to exit)\n",
         test_server.host_port_pair().ToString().c_str());

  message_loop.Run();
  return 0;
}