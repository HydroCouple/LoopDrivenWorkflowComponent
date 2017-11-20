#include "stdafx.h"
#include "loopdrivenworkflowcomponent.h"
#include "loopdrivenworkflowcomponentinfo.h"

using namespace HydroCouple;

LoopDrivenWorkflowComponent::LoopDrivenWorkflowComponent(const QString &id, LoopDrivenWorkflowComponentInfo *componentInfo):
  Identity(id, componentInfo)
{
  m_status = WorkflowStatus::Created;
}

LoopDrivenWorkflowComponent::~LoopDrivenWorkflowComponent()
{

}

HydroCouple::IWorkflowComponentInfo *LoopDrivenWorkflowComponent::componentInfo() const
{
  return m_loopWorkflowComponentInfo;
}

void LoopDrivenWorkflowComponent::initialize()
{
  if(m_status == WorkflowStatus::Created ||
     m_status == WorkflowStatus::Finished ||
     m_status == WorkflowStatus::Failed ||
     m_status == WorkflowStatus::Finished)
  {
    m_status = WorkflowStatus::Initializing;
    emit componentStatusChanged(m_status, "Initializing component...");

    m_timeModelComponents.clear();

    for(HydroCouple::IModelComponent *modelComponent : m_modelComponents)
    {
      HydroCouple::Temporal::ITimeModelComponent *timeModelComponent = dynamic_cast<HydroCouple::Temporal::ITimeModelComponent*>(modelComponent);

      if(timeModelComponent)
      {
        m_timeModelComponents.append(timeModelComponent);
      }
      else
      {
        m_status = WorkflowStatus::Failed;
        emit componentStatusChanged(m_status, "One of the components is not a time marching model");
      }
    }

    m_status = WorkflowStatus::Initialized;
    emit componentStatusChanged(m_status, "Initializing component...");
  }
}

void LoopDrivenWorkflowComponent::update()
{
  if(m_status == WorkflowStatus::Initialized || m_status == WorkflowStatus::Updated)
  {
    if(!allComponents(HydroCouple::IModelComponent::ComponentStatus::Done) &&
       allComponents(HydroCouple::IModelComponent::ComponentStatus::Updated) &&
       !atLeastOne(HydroCouple::IModelComponent::ComponentStatus::Failed))
    {
      double maxTime = getMaxTime();

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
      for(int i =  0; i < m_timeModelComponents.size(); i++)
      {
        HydroCouple::Temporal::ITimeModelComponent *timeModelComponent = m_timeModelComponents[i];

        if(timeModelComponent->currentDateTime()->modifiedJulianDay() <= maxTime)
          timeModelComponent->update();
      }
    }

    if(allComponents(HydroCouple::IModelComponent::ComponentStatus::Done))
    {
      m_status = WorkflowStatus::Done;
      emit componentStatusChanged(m_status, "Loop workflow done");
    }
    else if(atLeastOne(HydroCouple::IModelComponent::ComponentStatus::Failed))
    {
      m_status = WorkflowStatus::Failed;
      emit componentStatusChanged(m_status, "At least one component has failed");
    }
    else
    {
      m_status = WorkflowStatus::Updated;
      emit componentStatusChanged(m_status, "Loop workflow updated");
    }
  }
}

void LoopDrivenWorkflowComponent::finish()
{
  if(m_status == WorkflowStatus::Done ||
     m_status == WorkflowStatus::Failed)
  {
    m_status = WorkflowStatus::Finishing;
    emit componentStatusChanged(m_status, "Workflow component is finishing...");



    m_status = WorkflowStatus::Finished;
    emit componentStatusChanged(m_status, "Workflow component finished");
  }
}

IWorkflowComponent::WorkflowStatus LoopDrivenWorkflowComponent::status() const
{
  return m_status;
}

QList<HydroCouple::IModelComponent*> LoopDrivenWorkflowComponent::modelComponents() const
{
  return m_modelComponents;
}

void LoopDrivenWorkflowComponent::addModelComponent(HydroCouple::IModelComponent *component)
{
  if(m_status == WorkflowStatus::Created ||
     m_status == WorkflowStatus::Finished ||
     m_status == WorkflowStatus::Failed)
  {
    m_modelComponents.append(component);
  }
}

void LoopDrivenWorkflowComponent::removeModelComponent(HydroCouple::IModelComponent *component)
{
  if(m_status == WorkflowStatus::Created ||
     m_status == WorkflowStatus::Finished ||
     m_status == WorkflowStatus::Failed)
  {
    m_modelComponents.removeAll(component);
  }
}

bool LoopDrivenWorkflowComponent::allComponents(IModelComponent::ComponentStatus status)
{
  for(HydroCouple::Temporal::ITimeModelComponent *timeComponent : m_timeModelComponents)
  {
    if(timeComponent->status() != status)
      return false;
  }

  return true;
}

bool LoopDrivenWorkflowComponent::atLeastOne(IModelComponent::ComponentStatus status)
{
  for(HydroCouple::Temporal::ITimeModelComponent *timeComponent : m_timeModelComponents)
  {
    if(timeComponent->status() == status)
      return true;
  }

  return false;
}

double LoopDrivenWorkflowComponent::getMaxTime()
{
  double maxTime = std::numeric_limits<double>::min();

  for(HydroCouple::Temporal::ITimeModelComponent *timeComponent : m_timeModelComponents)
  {
    maxTime = std::max(maxTime, timeComponent->currentDateTime()->modifiedJulianDay());
  }

  return maxTime;
}






