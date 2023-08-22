using EnvDTE;
using Microsoft;
using Microsoft.VisualStudio.ComponentModelHost;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.TemplateWizard;
using NuGet.VisualStudio;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;

namespace SimpleKit.Templates.WindowsRuntime
{
    /// <summary>
    /// A wizard extension that adds a root namespace dedicated to C++
    /// type definitions.
    /// </summary>
    internal sealed class CppWizard : IWizard
    {
        [Import]
        internal IVsTemplateWizard Wizard { get; set; }

        private void Initialize(object automationObject)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            using (var serviceProvider = new ServiceProvider((IServiceProvider)automationObject))
            {
                var componentModel = (IComponentModel)serviceProvider.GetService(typeof(SComponentModel));
                Assumes.Present(componentModel);

                using (var container = new CompositionContainer(componentModel.DefaultExportProvider))
                {
                    container.ComposeParts(this);
                }
            }
        }

        // Code taken from here, many thanks to original author(s):
        // https://github.com/sylveon/cppwinrt/blob/bb2339505debce51f9aed13703227668537da0ba/vsix/WizardExtension.cs
        // Relevant thread: https://github.com/microsoft/cppwinrt/pull/918
        void IWizard.RunStarted(object automationObject, Dictionary<string, string> replacementsDictionary, WizardRunKind runKind, object[] customParams)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            Initialize(automationObject);

            if (replacementsDictionary.TryGetValue("$rootnamespace$", out var rootNamespace))
                replacementsDictionary.Add("$cpprootnamespace$", rootNamespace.Replace(".", "::"));
            else if (replacementsDictionary.TryGetValue("$projectname$", out var projectName))
                replacementsDictionary.Add("$cpprootnamespace$", projectName.Replace(".", "::"));

            Wizard.RunStarted(automationObject, replacementsDictionary, runKind, customParams);
        }

        void IWizard.BeforeOpeningFile(ProjectItem projectItem)
            => Wizard.BeforeOpeningFile(projectItem);

        void IWizard.ProjectFinishedGenerating(Project project)
            => Wizard.ProjectFinishedGenerating(project);

        void IWizard.ProjectItemFinishedGenerating(ProjectItem projectItem)
            => Wizard.ProjectItemFinishedGenerating(projectItem);

        void IWizard.RunFinished()
            => Wizard.RunFinished();

        bool IWizard.ShouldAddProjectItem(string filePath)
            => Wizard.ShouldAddProjectItem(filePath);
    }
}
