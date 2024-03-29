// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "mozart.hh"

namespace mozart {

//////////////////////
// PropertyRegistry //
//////////////////////

void PropertyRegistry::initialize(VM vm) {
  initConfig(vm);
  registerPredefined(vm);
}

void PropertyRegistry::initConfig(VM vm) {
  // Print

  config.printDepth = 10;
  config.printWidth = 20;

  // Errors

  config.defaultExceptionHandler = new (vm) StableNode;
  config.defaultExceptionHandler->init(vm, buildNil(vm));

  config.errorsDebug = true;
  config.errorsDepth = 10;
  config.errorsWidth = 20;
  config.errorsThread = 40;
}

void PropertyRegistry::registerPredefined(VM vm) {
  // Threads

  registerReadOnlyProp<nativeint>(vm, MOZART_STR("threads.runnable"),
    [] (VM vm) -> nativeint {
      return vm->getThreadPool().getRunnableCount();
    });

  // Print

  registerReadWriteProp(vm, MOZART_STR("print.depth"), config.printDepth);
  registerReadWriteProp(vm, MOZART_STR("print.width"), config.printWidth);

  // Errors

  registerProp(vm, MOZART_STR("errors.handler"),
    [this] (VM vm) -> UnstableNode {
      return { vm, *config.defaultExceptionHandler };
    },
    [this] (VM vm, RichNode value) {
      config.defaultExceptionHandler = value.getStableRef(vm);
    }
  );

  registerReadWriteProp(vm, MOZART_STR("errors.debug"), config.errorsDebug);
  registerReadWriteProp(vm, MOZART_STR("errors.depth"), config.errorsDepth);
  registerReadWriteProp(vm, MOZART_STR("errors.width"), config.errorsWidth);
  registerReadWriteProp(vm, MOZART_STR("errors.thread"), config.errorsThread);

  // Limits

  registerConstantProp(vm, MOZART_STR("limits.int.min"),
                       std::numeric_limits<nativeint>::min());
  registerConstantProp(vm, MOZART_STR("limits.int.max"),
                       std::numeric_limits<nativeint>::max());
  registerConstantProp(vm, MOZART_STR("limits.bytecode.xregisters"),
                       std::numeric_limits<ByteCode>::max());

  // Application (should be filled by the environment)

  registerValueProp(vm, MOZART_STR("application.args"), vm->coreatoms.nil);
  registerValueProp(vm, MOZART_STR("application.url"), vm->coreatoms.nil);
  registerValueProp(vm, MOZART_STR("application.gui"), false);

  // Platform (should be filled by the environment)

  registerValueProp(vm, MOZART_STR("platform.name"), MOZART_STR("unknown-unknown"));
  registerValueProp(vm, MOZART_STR("platform.os"), MOZART_STR("unknown"));
  registerValueProp(vm, MOZART_STR("platform.arch"), MOZART_STR("unknown"));

  // Misc

  registerValueProp(vm, MOZART_STR("oz.configure.home"), MOZART_STR("."));
  registerValueProp(vm, MOZART_STR("oz.emulator.home"), MOZART_STR("."));
  registerConstantProp(vm, MOZART_STR("oz.version"), MOZART_STR("2.0.0"));
  registerConstantProp(vm, MOZART_STR("oz.date"), MOZART_STR("20120721124706"));
}

}
