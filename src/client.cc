#include "client.h"

Persistent<FunctionTemplate> Client::constructor_template;
    
void Client::Initialize() {
    HandleScope scope;
    
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    constructor_template = Persistent<FunctionTemplate>::New(t);
    constructor_template->Inherit(EventEmitter::constructor_template);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    constructor_template->SetClassName(String::NewSymbol("Client"));
    
    NODE_SET_PROTOTYPE_METHOD(constructor_template, "end", End);
}

Handle<Value> Client::New(const Arguments &args) {
    HandleScope scope;
    return args.This();
}

Client::Client() {
    session = ssh_new();
    Wrap(
        constructor_template->GetFunction()->NewInstance()
    );
    Ref();
}

int Client::GetMessage(eio_req *req) {
    Client *client = (Client *) req->data;
    
printf("get message\n"); fflush(stdout);
    ssh_message msg = ssh_message_get(client->session);
printf("got message\n"); fflush(stdout);
    client->messageQueue.push_back(msg);
    
    return 0;
}

int Client::GetMessageAfter(eio_req *req) {
    HandleScope scope;
    Client *client = (Client *) req->data;
    
    eio_custom(GetMessage, EIO_PRI_DEFAULT, GetMessageAfter, client);
    ev_ref(EV_DEFAULT_UC);
    
    while (!client->messageQueue.empty()) {
        ssh_message msg = client->messageQueue.front();
        client->messageQueue.pop_front();
        
        Msg *m = new Msg(msg);
        Handle<Value> argv[1];
        argv[0] = m->handle_;
        client->Emit(String::NewSymbol("message"), 1, argv);
    }
    
    return 0;
}

Handle<Value> Client::End(const Arguments &args) {
    HandleScope scope;
    
    return args.This();
}
