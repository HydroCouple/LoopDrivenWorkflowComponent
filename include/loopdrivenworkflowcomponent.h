#ifndef LOOPDRIVENWORKFLOWCOMPONENT_H
#define LOOPDRIVENWORKFLOWCOMPONENT_H

#include "loopdrivenworkflowcomponent_global.h"
#include "hydrocouple.h"
#include "core/identity.h"
#include "hydrocoupletemporal.h"
#include <QVector>


class LoopDrivenWorkflowComponentInfo;

class LOOPDRIVENWORKFLOWCOMPONENT_EXPORT LoopDrivenWorkflowComponent : public Identity,
    public virtual HydroCouple::IWorkflowComponent
{
    Q_OBJECT
    Q_INTERFACES(HydroCouple::IWorkflowComponent)

  public:

    LoopDrivenWorkflowComponent(const QString &id, LoopDrivenWorkflowComponentInfo *componentInfo);

    virtual ~LoopDrivenWorkflowComponent();

    HydroCouple::IWorkflowComponentInfo *componentInfo() const override;

    void initialize() override;

    void update() override;

    void finish() override;

    WorkflowStatus status() const override;

    QList<HydroCouple::IModelComponent*> modelComponents() const override;

    void addModelComponent(HydroCouple::IModelComponent *component) override;

    void removeModelComponent(HydroCouple::IModelComponent *component) override;

  signals:

    void componentStatusChanged(WorkflowStatus status, const QString &message) override;

  private:

    bool allComponents(HydroCouple::IModelComponent::ComponentStatus status);

    bool atLeastOne(HydroCouple::IModelComponent::ComponentStatus status);

    double getMaxTime();

  private:

    LoopDrivenWorkflowComponentInfo *m_loopWorkflowComponentInfo;
    WorkflowStatus m_status;
    QList<HydroCouple::IModelComponent*> m_modelComponents;
    QVector<HydroCouple::Temporal::ITimeModelComponent*> m_timeModelComponents;
};


#endif // LOOPDRIVENWORKFLOWCOMPONENT_H
