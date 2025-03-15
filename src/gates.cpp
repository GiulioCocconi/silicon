#include <gates.hpp>

AndGate::AndGate(std::vector<Wire*> inputs, Wire* output) {
 this->inputs = inputs;
 this->output = output;

  for (Wire* w : this->inputs)
    w->addUpdateAction([&]() {
      State s = State::HIGH;

      for (Wire* input : this->inputs)
	s = s && input->getCurentState();
      
      this->output->setCurrentState(s);
    });
} 


OrGate::OrGate(std::vector<Wire*> inputs, Wire* output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire* w : this->inputs)
    w->addUpdateAction([&]() {
      State s = State::LOW;

      for (Wire* input : this->inputs)
	s = s || input->getCurentState();
      
      this->output->setCurrentState(s);
    });
} 

NotGate::NotGate(Wire* input, Wire* output) {
  this->inputs = { input };
  this->output = output;

    this->inputs[0]->addUpdateAction([&]() {
      this->output->setCurrentState(!this->inputs[0]->getCurentState());
    });
} 

NandGate::NandGate(std::vector<Wire*> inputs, Wire* output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire* w : this->inputs)
    w->addUpdateAction([&]() {
      Wire a("a");
      Wire n("n");
      AndGate ag(this->inputs, &a);
      NotGate ng(&a, &n);
      this->output->setCurrentState(n.getCurentState());
    });
}

NorGate::NorGate(std::vector<Wire*> inputs, Wire* output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire* w : this->inputs)
    w->addUpdateAction([&]() {
      Wire o("o");
      Wire n("n");
      OrGate  og(this->inputs, &o);
      NotGate ng(&o, &n);
      this->output->setCurrentState(n.getCurentState());
    });
}

XorGate::XorGate(std::vector<Wire*> inputs, Wire* output) {
  // We support only 2-bit Xors
  assert(inputs.size() == 2);

  this->inputs = inputs;
  this->output = output;

  for (Wire* w : this->inputs)
    w->addUpdateAction([&]() {
      Wire ow("ow");
      Wire nandw("nandw");
      Wire outw("outw");
      OrGate og({this->inputs[0], this->inputs[1]}, &ow);
      NandGate nandg({this->inputs[0], this->inputs[1]}, &nandw);
      AndGate andg({&ow, &nandw}, &outw);
      this->output->setCurrentState(outw.getCurentState());
    });
}
