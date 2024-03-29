// Copyright © 2012, Université catholique de Louvain
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

#ifndef __MODPORT_H
#define __MODPORT_H

#include "../mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

namespace builtins {

/////////////////
// Port module //
/////////////////

class ModPort: public Module {
public:
  ModPort(): Module("Port") {}

  class New: public Builtin<New> {
  public:
    New(): Builtin("new") {}

    void operator()(VM vm, Out stream, Out result) {
      result = Port::build(vm, stream);
    }
  };

  class Is: public Builtin<Is> {
  public:
    Is(): Builtin("is") {}

    void operator()(VM vm, In value, Out result) {
      result = build(vm, PortLike(value).isPort(vm));
    }
  };

  class Send: public Builtin<Send> {
  public:
    Send(): Builtin("send") {}

    void operator()(VM vm, In port, In value) {
      PortLike(port).send(vm, value);
    }
  };

  class SendReceive: public Builtin<SendReceive> {
  public:
    SendReceive(): Builtin("sendReceive") {}

    void operator()(VM vm, In port, In value, Out reply) {
      reply = PortLike(port).sendReceive(vm, value);
    }
  };
};

}

}

#endif // MOZART_GENERATOR

#endif // __MODPORT_H
