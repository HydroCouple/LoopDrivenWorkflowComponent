#ifndef LOOPDRIVENWORKFLOWCOMPONENTINFO_H
#define LOOPDRIVENWORKFLOWCOMPONENTINFO_H

#include "core/componentinfo.h"
#include "loopdrivenworkflowcomponent_global.h"



class LOOPDRIVENWORKFLOWCOMPONENT_EXPORT LoopDrivenWorkflowComponentInfo : public ComponentInfo,
    public virtual HydroCouple::IWorkflowComponentInfo
{
    Q_OBJECT
    Q_INTERFACES(HydroCouple::IWorkflowComponentInfo)
    Q_PLUGIN_METADATA(IID "LoopDrivenWorkflowComponentInfo")

  public:

    LoopDrivenWorkflowComponentInfo(QObject *parent = nullptr);

    virtual ~LoopDrivenWorkflowComponentInfo();

    HydroCouple::IWorkflowComponent *createComponentInstance() override;

};

#endif // LOOPDRIVENWORKFLOWCOMPONENTINFO_H
