/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

HoaToolsAudioProcessorEditor::HoaToolsAudioProcessorEditor(HoaToolsAudioProcessor* ownerFilter, HoaProcessor* aHoaProcessor): AudioProcessorEditor(ownerFilter)
{
    m_processor = ownerFilter;
    m_hoa_processor = aHoaProcessor;
    
    m_map       = new MapEditor(m_hoa_processor->getSourceManager(), ownerFilter);
    m_meter     = new MeterEditor(m_hoa_processor->getMeter());

    HoaFont.setHeight(11.);
    HoaFont.setBold(1);
    
    if(ownerFilter->getGui() == gui_mode_map)
    {
        addAndMakeVisible(m_map);
        m_map->setSize(495, 495);
        m_map->setBounds(2.5, 2.5, 495, 495);
    
        addAndMakeVisible(m_meter);
        m_meter->setSize(123, 123);
        m_meter->setBounds(501, 375, 124, 124);
    }
    else
    {        
        addAndMakeVisible(m_meter);
        m_meter->setSize(495, 495);
        m_meter->setBounds(2.5, 2.5, 495, 495);
        
        addAndMakeVisible(m_map);
        m_map->setSize(123, 123);
        m_map->setBounds(501, 375, 124, 124);
    }
    
    /////////////////////////////////////////////////
    /////////////////   ORDER   /////////////////////
    /////////////////////////////////////////////////
    addAndMakeVisible(m_order_text = new Label());
    m_order_text->setText(String("Order :"), NotificationType(0));
    m_order_text->setBounds(501, 5, 80, 15);
    m_order_text->setFont(HoaFont);

    addAndMakeVisible(m_order_menu = new Slider());
    m_order_menu->setSliderStyle(Slider::IncDecButtons);
    m_order_menu->setTextBoxIsEditable(1);
    m_order_menu->setTextBoxStyle(Slider::TextBoxLeft, false, 35, 11);
    m_order_menu->setBounds(581, 5, 50, 12);
    m_order_menu->setRange(0, 64, 1);
    m_order = (int)m_hoa_processor->getOrder();
    m_order_menu->getValueObject().referTo(m_order);
    m_order.addListener(this);
    
    /////////////////////////////////////////////////
    /////////////////   NINPUTS  ////////////////////
    /////////////////////////////////////////////////
    addAndMakeVisible(m_sources_text = new Label());
    m_sources_text->setText(String("Sources :"), NotificationType(0));
    m_sources_text->setBounds(501, 25, 80, 15);
    m_sources_text->setFont(HoaFont);
    
    addAndMakeVisible(m_sources_menu = new Slider());
    m_sources_menu->setSliderStyle(Slider::IncDecButtons);
    m_sources_menu->setTextBoxIsEditable(1);
    m_sources_menu->setTextBoxStyle(Slider::TextBoxLeft, false, 35, 11);
    m_sources_menu->setBounds(581, 25, 50, 12);
    m_sources_menu->setRange(0, 64, 1);
    m_nunber_of_sources = (int)m_hoa_processor->getNumberOfSources();
    m_sources_menu->getValueObject().referTo(m_nunber_of_sources);
    m_nunber_of_sources.addListener(this);
    
    /////////////////////////////////////////////////
    /////////////////   NOUTPUTS  ///////////////////
    /////////////////////////////////////////////////
    addAndMakeVisible(m_channels_text = new Label());
    m_channels_text->setText(String("Channels :"), NotificationType(0));
    m_channels_text->setBounds(501, 45, 80, 15);
    m_channels_text->setFont(HoaFont);
    
    addAndMakeVisible(m_channels_menu = new Slider());
    m_channels_menu->setSliderStyle(Slider::IncDecButtons);
    m_channels_menu->setTextBoxIsEditable(1);
    m_channels_menu->setTextBoxStyle(Slider::TextBoxLeft, false, 35, 11);
    m_channels_menu->setBounds(581, 45, 50, 12);
    m_channels_menu->setRange(0, 64, 1);
    m_nunber_of_channels = (int)m_hoa_processor->getNumberOfChannels();
    m_channels_menu->getValueObject().referTo(m_nunber_of_channels);
    m_nunber_of_channels.addListener(this);
    
    /////////////////////////////////////////////////
    /////////////////   DECMODE   ///////////////////
    /////////////////////////////////////////////////
    addAndMakeVisible(m_decoder_menu = new ComboBox());
    m_decoder_menu->setEditableText(false);
    m_decoder_menu->setJustificationType(Justification::centredLeft);
    m_decoder_menu->setTextWhenNoChoicesAvailable("(no choices)");
    m_decoder_menu->addItem("Decoding : Ambisonic", 1);
    m_decoder_menu->addItem("Decoding : Binaural", 2);
    m_decoder_menu->addItem("Decoding : Irregular", 3);
    m_decoder_menu->addListener(this);
    m_decoder_menu->setSelectedId(m_hoa_processor->getDecodingMode()+1);
    m_decoder_menu->setBounds(501, 77, 122, 15);
    m_decoder_menu->setLookAndFeel(&LookAndFeel);
    
    /////////////////////////////////////////////////
    /////////////////   OPTIM   /////////////////////
    /////////////////////////////////////////////////
    addAndMakeVisible(m_optim_menu = new ComboBox());
    m_optim_menu->setEditableText(false);
    m_optim_menu->setJustificationType(Justification::centredLeft);
    m_optim_menu->setTextWhenNoChoicesAvailable("(no choices)");
    m_optim_menu->addItem("Optim : Basic", 1);
    m_optim_menu->addItem("Optim : MaxRe", 2);
    m_optim_menu->addItem("Optim : InPhase", 3);
    m_optim_menu->addListener(this);
    m_optim_menu->setSelectedId(m_hoa_processor->getOptimization()+1);
    m_optim_menu->setBounds(501, 92, 122, 15);
    m_optim_menu->setLookAndFeel(&LookAndFeel);    
    
    addAndMakeVisible(m_offset_text = new Label("Offset"));
    m_offset_text->setText(String("Offset of channels"), NotificationType(1));
    m_offset_text->setBounds(501, 107, 122, 15);
    addAndMakeVisible(m_offset_menu = new Label("Offset"));
    m_offset_menu->setText(String(0), NotificationType(1));
    m_offset_menu->setEditable(1);
    m_offset_menu->setBounds(501, 122, 122, 15);
   
    addAndMakeVisible(m_switch    = new ShapeButton(String(""), Colour::fromRGBA(0,0,0,0), Colour::fromRGBA(50, 50, 50, 50), Colour::fromRGBA(50, 50, 50, 125)));
    m_switch->setSize(123, 123);
    m_switch->setBounds(501, 375, 124, 124);
    m_switch->addListener(this);    
    
    setSize(625, 500);
}

void HoaToolsAudioProcessorEditor::paint(Graphics& g)
{
    g.setColour(Colours::lightgrey);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 5);
    g.setColour(Colours::grey);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 5, 5.);
    
    g.setColour(Colours::grey);
    g.drawLine(499., 1., 498., getHeight() - 2., 2.5);
    g.setColour(Colours::grey);
    g.drawLine(500, 374, 625, 374, 2.5);
    g.setColour(Colours::grey);
    g.drawLine(500, 124, 625, 124, 2.5);
}

HoaToolsAudioProcessorEditor::~HoaToolsAudioProcessorEditor()
{
    delete m_map;
    delete m_meter;
    delete m_optim_menu;
    delete m_order_menu;
    delete m_offset_menu;
    delete m_decoder_menu;
    delete m_switch;
}

void HoaToolsAudioProcessorEditor::valueChanged(Value& aValue)
{
    if(aValue == m_nunber_of_sources)
    {
        m_hoa_processor->setNumberOfSources(m_sources_menu->getValue());
    }
    else if(aValue == m_nunber_of_channels)
    {
        m_hoa_processor->setNumberOfChannels(m_channels_menu->getValue());
    }
}

void HoaToolsAudioProcessorEditor::comboBoxChanged(ComboBox* aComboBox)
{
    if(aComboBox == m_optim_menu)
    {
        m_hoa_processor->setOptimization(m_optim_menu->getSelectedId()-1);
    }
    else if(aComboBox == m_decoder_menu)
    {
        ;
    }
}


void HoaToolsAudioProcessorEditor::buttonClicked(Button* aButton)
{
    if(m_processor->getGui() == gui_mode_map)
    {
        m_meter->setSize(495, 495);
        m_meter->setBounds(2.5, 2.5, 495, 495);
        m_map->setSize(123, 123);
        m_map->setBounds(501, 375, 124, 124);
        m_processor->setGui(gui_mode_meter);
    }
    else
    {
        m_map->setSize(495, 495);
        m_map->setBounds(2.5, 2.5, 495, 495);
        m_meter->setSize(123, 123);
        m_meter->setBounds(501, 375, 124, 124);
        m_processor->setGui(gui_mode_map);
    }
}




