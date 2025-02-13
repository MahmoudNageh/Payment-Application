import json
import xml.etree.ElementTree as ET
import uuid

def json_to_arxml(json_data, arxml_filename):
    # Create the root element for the ARXML
    containers = ET.Element('CONTAINERS')

    # Function to recursively add elements
    def add_elements(parent, data, path):
        for key, value in data.items():
            if isinstance(value, dict):
                # Create a new container for nested dictionaries
                container = ET.SubElement(parent, 'ECUC-CONTAINER-VALUE')
                container.set('UUID', str(uuid.uuid4()))  # Generate a unique UUID
                short_name = ET.SubElement(container, 'SHORT-NAME')
                short_name.text = key
                definition_ref = ET.SubElement(container, 'DEFINITION-REF')
                definition_ref.set('DEST', 'ECUC-PARAM-CONF-CONTAINER-DEF')
                definition_ref.text = f'/Hamada{path}/{key}'
                # Recursively add nested elements
                add_elements(container, value, f'{path}/{key}')
            else:
                # Handle parameter values
                if value['type'] == 'BOOLEAN':
                    param_value = ET.SubElement(parent, 'ECUC-NUMERICAL-PARAM-VALUE')
                    definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                    definition_ref.set('DEST', 'ECUC-BOOLEAN-PARAM-DEF')
                    definition_ref.text = f'/Hamada{path}/{key}'
                    val = ET.SubElement(param_value, 'VALUE')
                    val.text = value['value']
                elif value['type'] == 'ENUMERATION':
                    param_value = ET.SubElement(parent, 'ECUC-TEXTUAL-PARAM-VALUE')
                    definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                    definition_ref.set('DEST', 'ECUC-ENUMERATION-PARAM-DEF')
                    definition_ref.text = f'/Hamada{path}/{key}'
                    val = ET.SubElement(param_value, 'VALUE')
                    val.text = value['value']
                elif value['type'] == 'INTEGER':
                    param_value = ET.SubElement(parent, 'ECUC-NUMERICAL-PARAM-VALUE')
                    definition_ref = ET.SubElement(param_value, 'DEFINITION-REF')
                    definition_ref.set('DEST', 'ECUC-INTEGER-PARAM-DEF')
                    definition_ref.text = f'/Hamada{path}/{key}'
                    val = ET.SubElement(param_value, 'VALUE')
                    val.text = value['value']
                elif value['type'] == 'ref':
                    reference_value = ET.SubElement(parent, 'ECUC-REFERENCE-VALUE')
                    definition_ref = ET.SubElement(reference_value, 'DEFINITION-REF')
                    definition_ref.set('DEST', 'ECUC-REFERENCE-DEF')
                    definition_ref.text = f'/Hamada{path}/{key}'
                    value_ref = ET.SubElement(reference_value, 'VALUE-REF')
                    value_ref.set('DEST', 'ECUC-CONTAINER-VALUE')
                    value_ref.text = value['value']

    # Start the recursive addition from the root
    add_elements(containers, json_data, '')

    # Convert the XML tree to a string and write to file
    tree = ET.ElementTree(containers)
    tree.write(arxml_filename, encoding='utf-8', xml_declaration=True)

# Example usage
if __name__ == "__main__":
    # Load JSON data from a file
    with open('input.json', 'r') as file:
        json_data = json.load(file)
    
    # Convert JSON to ARXML and save to file
    json_to_arxml(json_data, 'output.arxml')