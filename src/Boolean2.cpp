#include "HetrickCV.hpp"
#include "dsp/digital.hpp"

struct Boolean2 : Module 
{
	enum ParamIds 
	{
		NUM_PARAMS
	};
	enum InputIds 
	{
        INA_INPUT,
        INB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds 
	{
        OR_OUTPUT,
        AND_OUTPUT,
        XOR_OUTPUT,
        NOR_OUTPUT,
        NAND_OUTPUT,
        XNOR_OUTPUT,
		NUM_OUTPUTS
	};

    bool inA = false;
    bool inB = false;
    float outs[6] = {};
    float lightA = 0.0f, lightB = 0.0f;

	Boolean2() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) 
	{
		
	}

	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - reset, randomize: implements special behavior when user clicks these from the context menu
};


void Boolean2::step() 
{
    inA = (inputs[INA_INPUT].value >= 1.0f);
    inB = (inputs[INB_INPUT].value >= 1.0f);

    lightA = inA ? 5.0f : 0.0f;
    lightB = inB ? 5.0f : 0.0f;

    outs[0] = (inA || inB) ? 5.0f : 0.0f;
    outs[1] = (inA && inB) ? 5.0f : 0.0f;
    outs[2] = (inA != inB) ? 5.0f : 0.0f;
    outs[3] = 5.0f - outs[0];
    outs[4] = 5.0f - outs[1];
    outs[5] = 5.0f - outs[2];

    outputs[OR_OUTPUT].value = outs[0];
    outputs[AND_OUTPUT].value = outs[1];
    outputs[XOR_OUTPUT].value = outs[2];
    outputs[NOR_OUTPUT].value = outs[3];
    outputs[NAND_OUTPUT].value = outs[4];
    outputs[XNOR_OUTPUT].value = outs[5];
}


Boolean2Widget::Boolean2Widget() 
{
	auto *module = new Boolean2();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		auto *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Boolean.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 365)));

    //////PARAMS//////

    //////INPUTS//////
    addInput(createInput<PJ301MPort>(Vec(10, 105), module, Boolean2::INA_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10, 195), module, Boolean2::INB_INPUT));

    //////OUTPUTS//////
    addOutput(createOutput<PJ301MPort>(Vec(45, 60), module, Boolean2::OR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 105), module, Boolean2::AND_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 150), module, Boolean2::XOR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 195), module, Boolean2::NOR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 240), module, Boolean2::NAND_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 285), module, Boolean2::XNOR_OUTPUT));

    //////BLINKENLIGHTS//////
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 68), &module->outs[0]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 113), &module->outs[1]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 158), &module->outs[2]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 203), &module->outs[3]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 248), &module->outs[4]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(74, 293), &module->outs[5]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(18, 92), &module->lightA));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(18, 182), &module->lightB));
}
