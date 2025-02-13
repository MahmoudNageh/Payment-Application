import json
import xml.etree.ElementTree as ET
import uuid
from xml.dom import minidom

def json_to_arxml(json_data, arxml_filename):
    # Create the root element for the ARXML
    containers = ET.Element('CONTAINERS')

    # Function to recursively add elements
    def add_elements(parent, data, path, depth):
        # Create containers for PARAMETER-VALUES and REFERENCE-VALUES if needed
        parameter_values = None
        reference_values = None
        sub_containers = None

        for key, value in data.items():
            if isinstance(value, dict):
                # Check if this is a parameter (has 'type' and 'value' keys)
                if 'type' in value and 'value' in value:
                    # Handle parameter values
                    if parameter_values is None:
                        parameter_values = ET.SubElement(parent, 'PARAMETER-VALUES')

                    if value['type'] == 'BOOLEAN':
                        param_value = ET.SubElement(parameter_values, 'ECUC-NUMERICAL-PARAM-VALUE')
                        definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                        definition_ref.set('DEST', 'ECUC-BOOLEAN-PARAM-DEF')
                        definition_ref.text = f'/Hamada{path}/{key}'
                        val = ET.SubElement(param_value, 'VALUE')
                        val.text = value['value']
                    elif value['type'] == 'ENUMERATION':
                        param_value = ET.SubElement(parameter_values, 'ECUC-TEXTUAL-PARAM-VALUE')
                        definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                        definition_ref.set('DEST', 'ECUC-ENUMERATION-PARAM-DEF')
                        definition_ref.text = f'/Hamada{path}/{key}'
                        val = ET.SubElement(param_value, 'VALUE')
                        val.text = value['value']
                    elif value['type'] == 'INTEGER':
                        param_value = ET.SubElement(parameter_values, 'ECUC-NUMERICAL-PARAM-VALUE')
                        definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                        definition_ref.set('DEST', 'ECUC-INTEGER-PARAM-DEF')
                        definition_ref.text = f'/Hamada{path}/{key}'
                        val = ET.SubElement(param_value, 'VALUE')
                        val.text = value['value']
                    elif value['type'] == 'ref':
                        if reference_values is None:
                            reference_values = ET.SubElement(parent, 'REFERENCE-VALUES')
                        reference_value = ET.SubElement(reference_values, 'ECUC-REFERENCE-VALUE')
                        definition_ref = ET.SubElement(reference_value, 'DEFINITION-REF')
                        definition_ref.set('DEST', 'ECUC-REFERENCE-DEF')
                        definition_ref.text = f'/Hamada{path}/{key}'
                        value_ref = ET.SubElement(reference_value, 'VALUE-REF')
                        value_ref.set('DEST', 'ECUC-CONTAINER-VALUE')
                        value_ref.text = value['value']
                else:
                    # Create a new container for nested dictionaries
                    if depth == 1:
                        # Only create SUB-CONTAINERS at depth 1 (under B)
                        if sub_containers is None:
                            sub_containers = ET.SubElement(parent, 'SUB-CONTAINERS')
                        container = ET.SubElement(sub_containers, 'ECUC-CONTAINER-VALUE')
                    else:
                        container = ET.SubElement(parent, 'ECUC-CONTAINER-VALUE')

                    container.set('UUID', str(uuid.uuid4()))  # Generate a unique UUID
                    short_name = ET.SubElement(container, 'SHORT-NAME')
                    short_name.text = key
                    definition_ref = ET.SubElement(container, 'DEFINITION-REF')
                    definition_ref.set('DEST', 'ECUC-PARAM-CONF-CONTAINER-DEF')
                    definition_ref.text = f'/Hamada{path}/{key}'

                    # Recursively add nested elements
                    add_elements(container, value, f'{path}/{key}', depth + 1)
            else:
                print(f"Skipping invalid entry: {key} = {value}")  # Debug print

    # Start the recursive addition from the root
    add_elements(containers, json_data, '', depth=0)

    # Convert the XML tree to a string
    xml_str = ET.tostring(containers, encoding='utf-8')

    # Use minidom to pretty-print the XML
    dom = minidom.parseString(xml_str)
    pretty_xml = dom.toprettyxml(indent="  ")

    # Write the pretty-printed XML to the output file
    with open(arxml_filename, 'w', encoding='utf-8') as file:
        file.write(pretty_xml)

# Example usage
if __name__ == "__main__":
    # Load JSON data from a file
    with open('example_json.json', 'r') as file:
        json_data = json.load(file)
        print("Loaded JSON:", json.dumps(json_data, indent=2))  # Debug print
    
    # Convert JSON to ARXML and save to file
    json_to_arxml(json_data, 'output.arxml')
