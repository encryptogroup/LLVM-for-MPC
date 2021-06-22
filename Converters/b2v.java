package de.tu_darmstadt.cs.encrypto.compiler;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class b2v {

	public static void main(String[] args) throws IOException {

		String circuitfilename = args[0];

		List<String> bristolInput = Files.readAllLines(Paths.get(circuitfilename + ".bristol"), StandardCharsets.UTF_8);

		BufferedWriter writer = Files.newBufferedWriter(Paths.get(circuitfilename + ".v"), StandardCharsets.UTF_8);

		// process number of gates and wires
		String[] gatesAndWireSpecification = bristolInput.remove(0).split(" ");
		int numberOfGates = Integer.valueOf(gatesAndWireSpecification[0]);
		int numberOfWires = Integer.valueOf(gatesAndWireSpecification[1]);

		// process number of input and output wires
		String[] inputAndOutputSpecification = bristolInput.remove(0).split(" ");
		int numberWiresIn = 0;
		for (int i = 0; i < inputAndOutputSpecification.length - 1; i++) {
			numberWiresIn += Integer.valueOf(inputAndOutputSpecification[i]);
		}
		int numberWiresOut = Integer.valueOf(inputAndOutputSpecification[inputAndOutputSpecification.length - 1]);

		// declare module with input and output logic
		writer.write("module " + circuitfilename + "(input ");
		for (int i = 0; i < numberWiresIn; i++) {
			writer.write("w" + i + ", ");
		}
		writer.newLine();
		writer.write("\t" + "output ");
		for (int i = numberOfWires - numberWiresOut; i < numberOfWires; i++) {
			writer.write("w" + i + (i == numberOfWires - 1 ? "" : ", "));
		}
		writer.write(");");
		writer.newLine();
		writer.newLine();

		// declare internal wires if necessary
		if (numberOfWires > numberWiresIn + numberWiresOut) {
			writer.write("\t" + "wire ");
			for (int i = numberWiresIn; i < numberOfWires - numberWiresOut; i++) {
				writer.write("w" + i + (i == numberOfWires - numberWiresOut - 1 ? ";" : ", "));
			}
			writer.newLine();
			writer.newLine();
		}

		// drop empty line
		bristolInput.remove(0);

		for (String line : bristolInput) {
			String[] parsedLine = line.split(" ");

			// handle special case of inverter
			if (parsedLine.length == 5) {
				int inputWire = Integer.valueOf(parsedLine[2]);
				int outputWire = Integer.valueOf(parsedLine[3]);

				writer.write("\t" + "assign w" + outputWire + " = ~w" + inputWire + ";");
				writer.newLine();
			} else {
				int inputWire1 = Integer.valueOf(parsedLine[2]);
				int inputWire2 = Integer.valueOf(parsedLine[3]);
				int outputWire = Integer.valueOf(parsedLine[4]);

				String operator = parsedLine[5];

				String operatorSymbol = null;
				switch (operator) {
				case "XOR":
					operatorSymbol = "^";
					break;

				case "AND":
					operatorSymbol = "&";
					break;

				case "OR":
					operatorSymbol = "|";
					break;

				default:
					System.out.println("Unknown gate type.");
					break;
				}

				writer.write("\t" + "assign w" + outputWire + " = w" + inputWire1 + " " + operatorSymbol + " w"
						+ inputWire2 + ";");
				writer.newLine();
			}

		}

		// end module
		writer.newLine();
		writer.write("endmodule");
		writer.newLine();

		writer.close();
	}

}
