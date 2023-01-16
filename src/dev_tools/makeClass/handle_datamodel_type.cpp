/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
string_q type_2_GoType(const CParameter& field) {
    string_q type = field.type;
    if (startsWith(type, 'C')) {
        return type_2_ModelName(type, false);
    }
    if (type == "blknum")
        return "uint64";
    if (type == "timestamp")
        return "int64";
    if (type == "hash" || type == "bytes32")
        return "common.Hash";
    if (type == "datetime" || type == "bytes")
        return "string";
    if (type == "address")
        return "common.Address";
    if (type == "gas")
        return "Gas";
    if (type == "wei")
        return "Wei";
    return type;
}

string_q specialCase(const string_q& name, const string_q& type, bool isRaw) {
    if (name % "CumulativeGasUsed" && !isRaw) {
        return "string";
    }
    if (name % "Logs") {
        return isRaw ? "[]RawLog" : "[]SimpleLog";
    }
    return isRaw ? "string" : type;
}

string_q debug(const CParameter& field) {
    ostringstream os;
    // os << " //";
    // os << " doc: " << field.doc;
    // os << " disp: " << field.disp;
    // os << " omit: " << (field.is_flags & IS_OMITEMPTY);
    return os.str();
}

//------------------------------------------------------------------------------------------------------------
void generate_go_type_code(COptions* opts, const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;

    string_q fn = getPathToSource("apps/chifra/pkg/types/types_" + toLower(model.base_name) + ".go");
    string_q contents = asciiFileToString(getPathToTemplates("blank_type.go.tmpl"));
    replaceAll(contents, "[{CLASS_NAME}]", type_2_ModelName(model.gogen, false));

    CParameter raw;
    raw.type = "*Raw" + type_2_ModelName(model.gogen, false);
    raw.name = "raw";
    model.fieldArray.push_back(raw);

    size_t maxNameWid = 0, maxSimpWid = 0, maxRawWid = 0;
    for (auto& field : model.fieldArray) {
        string_q type = type_2_GoType(field);
        string_q rawType = specialCase(field.name, type, true);
        string_q simpType = specialCase(field.name, type, false);
        maxNameWid = max(maxNameWid, field.name.length());
        if (field.name != "raw") {
            maxSimpWid = max(maxSimpWid, simpType.length());
            maxRawWid = max(maxRawWid, rawType.length());
            field.name = firstUpper(field.name);
        }
    }

    string_q rawStr;
    for (auto field : model.fieldArray) {
        if (!(field.name % "raw")) {
            string_q type = type_2_GoType(field);
            string_q spec = specialCase(field.name, type, true);
            string_q rawType = field.name % "raw" ? spec : padRight(spec, maxRawWid);
            ostringstream os;
            os << "\t";
            os << padRight(field.name, maxNameWid) << " " << rawType;
            os << " `json:\"" << firstLower(field.name) << "\"`" << debug(field) << endl;
            rawStr += os.str();
        }
    }

    string_q fieldStr;
    for (auto field : model.fieldArray) {
        string_q type = type_2_GoType(field);
        string_q spec = specialCase(field.name, type, false);
        string_q simpType = (field.name % "raw") ? spec : padRight(spec, maxSimpWid);
        ostringstream os;
        os << "\t" << padRight(field.name, maxNameWid) << " " << simpType;
        if (!(field.name % "raw")) {
            os << " `json:\"" << firstLower(field.name) << (field.is_flags & IS_OMITEMPTY ? ",omitempty" : "") << "\"`"
               << debug(field);
        }
        os << endl;
        fieldStr += os.str();
    }

    string_q modelStr;
    for (auto field : model.fieldArray) {
        ostringstream os;
        if (!(field.name % "raw") && !(field.is_flags & (IS_OMITEMPTY | IS_ARRAY))) {
            os << "\t\t" << padRight("\"" + firstLower(field.name) + "\":", maxNameWid + 3) << " s."
               << firstUpper(field.name) << "," << debug(field) << endl;
        }
        modelStr += os.str();
    }

    string_q orderStr;
    for (auto field : model.fieldArray) {
        ostringstream os;
        if (!(field.name % "raw") && !(field.is_flags & (IS_OMITEMPTY | IS_ARRAY))) {
            os << "\t\t\"" << firstLower(field.name) << "\"," << debug(field) << endl;
        }
        orderStr += os.str();
    }

    replaceAll(contents, "[{RAWFIELDS}]", rawStr);
    replaceAll(contents, "[{FIELDS}]", fieldStr);
    replaceAll(contents, "[{MODEL_FIELDS}]", modelStr);
    replaceAll(contents, "[{ORDER_FIELDS}]", orderStr);

    codewrite_t cw(fn, contents + "\n");
    cw.nSpaces = 0;
    writeCodeIn(opts, cw);
    // cerr << fn << endl;
}
