from lxml import etree
import os

prj_file = 'example_oF_subplugin'
path_hotjuice_win = 'D:/Developer/Kiberchaika/hotjuice/src/'
path_resources = 'D:/Developer/Crossplatform/of_v0.10.1_vs2017_release/addons/ofxHotJuice/resources'
company_name = "MyCompany"
bundle_id = 'com.company.application'

tree = etree.parse(prj_file + '.vcxproj')

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
tree.write(prj_file + '.vcxproj')