#ifndef NODE_SSH_MESSAGE_H
#define NODE_SSH_MESSAGE_H

#define protected public

#include <node.h>
#include <node_object_wrap.h>
#include <node_events.h>
#include <v8.h>

#include <libssh/libssh.h>
#include <libssh/server.h>
#include <libssh/callbacks.h>

#include "chan.h"

using namespace v8;
using namespace node;

class Msg : public ObjectWrap {
public:
    ssh_message message;
    Msg(ssh_message);
    
    static Persistent<FunctionTemplate> constructor_template;
    static Handle<Value> New(const Arguments &);
    static Handle<Value> ReplyDefault(const Arguments &);
    static Handle<Value> AuthSetMethods(const Arguments &);
    static Handle<Value> AuthReplySuccess(const Arguments &);
    static Handle<Value> OpenChannel(const Arguments &);
    static Handle<Value> ChannelReplySuccess(const Arguments &);
    static void Initialize();
};
    
#endif
