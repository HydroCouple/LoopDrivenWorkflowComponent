#include "stdafx.h"
#include "loopdrivenworkflowcomponentinfo.h"
#include "loopdrivenworkflowcomponent.h"

LoopDrivenWorkflowComponentInfo::LoopDrivenWorkflowComponentInfo(QObject *parent):
  ComponentInfo(parent)
{
  setId("Loop Driven Workflow Component 1.0.0");
  setCaption("Loop Driven Workflow Component");
  setDescription("A loop driven workflow component for data exchange between time marching components.");
  setCategory("Time Marching");
  setCopyright("");
  setVendor("H");
  setUrl("https://hydrocouple.org");
  setEmail("caleb.buahin@gmail.com");
  setVersion("1.0.0");
}

LoopDrivenWorkflowComponentInfo::~LoopDrivenWorkflowComponentInfo()
{

}

HydroCouple::IWorkflowComponent *LoopDrivenWorkflowComponentInfo::createComponentInstance()
{
  QString id =  QUuid::createUuid().toString();
  LoopDrivenWorkflowComponent *component = new LoopDrivenWorkflowComponent(id,this);
  component->setDescription("Loop driven data exchange workflow component for time marching models");
  return component;
}
