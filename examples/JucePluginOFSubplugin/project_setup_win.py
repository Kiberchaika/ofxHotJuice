from lxml import etree
import os
from project_settings import *

path_to_project = os.path.join(project_name, project_name + '.vcxproj')
tree = etree.parse(path_to_project)

namespaces = {'ns':'http://schemas.microsoft.com/developer/msbuild/2003'}

for el in tree.xpath('//ns:ClCompile/ns:AdditionalIncludeDirectories', namespaces=namespaces):
    el.text = el.text + ";" + path_hotjuice_win

for el in tree.xpath('//ns:PreprocessorDefinitions', namespaces=namespaces):
    el.text = 'MURKA_OF;MURKA_OFFSCREEN;' + el.text

for el in tree.xpath('//ns:ConfigurationType', namespaces=namespaces):
    el.text = 'DynamicLibrary'

el = tree.xpath('//ns:ItemGroup/ns:ClCompile/..', namespaces=namespaces)[0]
for file in os.listdir(path_hotjuice_win):
    if file.endswith('.cpp'):
        el.append(etree.XML('<ClCompile Include="' +  os.path.join(path_hotjuice_win, file) + '"/>'))

el = tree.xpath('//ns:ItemGroup/ns:ClInclude/..', namespaces=namespaces)[0]
for file in os.listdir(path_hotjuice_win):
    if file.endswith(".h"):
        el.append(etree.XML('<ClInclude Include="' +  os.path.join(path_hotjuice_win, file) + '"/>'))

path_to_plugin = os.path.join(os.environ.get("APPDATA"), company_name, bundle_id)
for el in tree.xpath('//ns:PostBuildEvent', namespaces=namespaces):
    el.append(etree.XML('<Command>' +
    'mkdir ' + '"' + path_to_plugin + '" ' +
    ' &amp; ' + 'robocopy "' + path_resources + '" "' + path_to_plugin + '/resources/" /E '  +
    ' &amp; ' + 'robocopy "$(ProjectDir)bin/" "' + path_to_plugin + '/" "*.dll" "*.pdb" /is '  +
    ' &amp; ' + 'exit 0' +
    '</Command>'))

#tree.write('example_oF_subplugin.vcxproj')
tree.write(path_to_project)