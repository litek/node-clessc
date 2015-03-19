#include <nan.h>

#include <fstream>
#include <sstream>

#include "LessTokenizer.h"
#include "LessParser.h"
#include "CssWriter.h"
#include "CssPrettyWriter.h"
#include "Stylesheet.h"
#include "LessStylesheet.h"

using namespace v8;

class LessWorker : public NanAsyncWorker {
  public:
    LessWorker(NanCallback *callback, string source)
      : NanAsyncWorker(callback), source(source) {}
    ~LessWorker() {}

  void Execute() {
    ostringstream out;
    istream *in = new std::ifstream(source.c_str());

    if (in->fail() || in->bad()) {
      err = "Error opening file";
      return;
    }

    LessStylesheet less;
    Stylesheet css;
    ProcessingContext context;
    // CssWriter *writer = new CssPrettyWriter(out);
    CssWriter *writer = new CssWriter(out);

    std::list<std::string> sources;
    std::list<std::string>::iterator i;
  
    LessTokenizer tokenizer(*in, source);
    LessParser parser(tokenizer, sources);
    sources.push_back(source);

    try {
      parser.parseStylesheet(less);
      less.process(css, context);
      css.write(*writer);
      delete writer;

      res = out.str();

    } catch(ParseException* e) {
      err = e->what();
      return;

    } catch(exception* e) {
      err = e->what();
      return;
    }
  }

  void HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
      NanNew<String>(err),
      NanNew<String>(res)
    };

    callback->Call(2, argv);
  }

  private:
    string source;
    string err;
    string res;
};

NAN_METHOD(CompileFile) {
  NanScope();

  v8::String::Utf8Value arg1(args[0]->ToString());
  std::string source = std::string(*arg1);

  NanCallback *callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new LessWorker(callback, source));
  NanReturnUndefined();
}

void Init(Handle<Object> exports) {
  exports->Set(NanNew<String>("compileFile"),
    NanNew<FunctionTemplate>(CompileFile)->GetFunction());
}

NODE_MODULE(addon, Init)
