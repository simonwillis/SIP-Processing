//
// Created by Simon Willis on 25/02/2017.
//


#include "../src/components/utilities/XmlDocument.h"

#define SIPRECINVITE "<?xml version=\"1.0\" encoding=\"UTF-8\"?><recording_metadata xmlns=\"urn:ietf:params:xml:ns:siprec\"><dataMode>complete</dataMode><recording id=\"urn:uuid:5effa7c7-99d3-43a2-9bcb-390daa2017ec\"><requestor>SRC</requestor><type>selective</type></recording><group id=\"urn:uuid:a902bc72-7db1-4c18-8cac-971a20fc2b6a\" recording=\"urn:uuid:5effa7c7-99d3-43a2-9bcb-390daa2017ec\"><initiator>sip:Cognia-CRUser1@as.iop1.broadworks.net</initiator></group><session id=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\" group=\"urn:uuid:a902bc72-7db1-4c18-8cac-971a20fc2b6a\"><start-time>2017-03-29T13:56:13-04:00</start-time></session><participant id=\"urn:uuid:76f3144e-59cf-4078-b084-92edde584a3e\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><aor>sip:Cognia-CRUser1@as.iop1.broadworks.net</aor><send><id>urn:uuid:2566420c-3397-4e7b-84bc-38fbb276de11</id></send></participant><participant id=\"urn:uuid:5f035d42-2cfc-4932-9285-76e6b19ad37c\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><aor>sip:7663@as.iop1.broadworks.net;user=phone</aor><send><id>urn:uuid:c5f5819a-d8b7-47cb-ab58-8d969ab151cc</id></send></participant><participant id=\"urn:uuid:11ad2f51-c8ea-4845-aeed-4cc023be9748\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><aor>Cognia-CRBTBCUser1@as.iop1.broadworks.net</aor><send><id>urn:uuid:c5f5819a-d8b7-47cb-ab58-8d969ab151cc</id></send></participant><participant id=\"urn:uuid:8ab7746f-c7e3-4f99-a06e-8a36f1135401\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><aor>Cognia-CRBTBCUser2@as.iop1.broadworks.net</aor><send><id>urn:uuid:c5f5819a-d8b7-47cb-ab58-8d969ab151cc</id></send></participant><participant id=\"urn:uuid:17a45bd9-7358-419a-989b-abbba99bb160\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><aor>Cognia-CRBTBCUser3@as.iop1.broadworks.net</aor><send><id>urn:uuid:c5f5819a-d8b7-47cb-ab58-8d969ab151cc</id></send></participant><stream id=\"urn:uuid:2566420c-3397-4e7b-84bc-38fbb276de11\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><label>1</label><mode>separate</mode></stream><stream id=\"urn:uuid:c5f5819a-d8b7-47cb-ab58-8d969ab151cc\" session=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><label>2</label><mode>separate</mode></stream><extensiondata id=\"urn:uuid:3f6c043c-5004-425f-ac09-8d8ed53bf3ce\" parent=\"urn:uuid:759af1dc-27f7-42bb-8eca-3985d62c9458\"><broadWorksRecordingMetadata xmlns=\"http://schema.broadsoft.com/broadworksCallRecording\" version=\"3.0\"><extTrackingID>152463:1</extTrackingID><serviceProviderID>Interoperability</serviceProviderID><groupID>Cognia-CR</groupID><userID>Cognia-CRUser1@as.iop1.broadworks.net</userID><callID>callhalf-177056759:5</callID><callType><origCall><callingPartyNumber>sip:+12404987661@as.iop1.broadworks.net</callingPartyNumber><calledPartyNumber>sip:7663@as.iop1.broadworks.net;user=phone</calledPartyNumber><dialedDigits>sip:7663@as.iop1.broadworks.net</dialedDigits></origCall></callType><recordingType>on</recordingType></broadWorksRecordingMetadata></extensiondata></recording_metadata>"

bool test1() {
    const char * buf =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \r\n <recording_metadata xmlns=\"urn:ietf:params:xml:ns:siprec\"><dataMode>complete</dataMode><recording id=\"urn:uuid:727585d8-a8f2-43e0-8bd5-7795f2a7fca0\"><requestor>SRC</requestor><type>selective</type></recording><group id=\"urn:uuid:72561964-fe38-4eb5-be01-903ca224ca06\" recording=\"urn:uuid:727585d8-a8f2-43e0-8bd5-7795f2a7fca0\"><initiator>sip:sub_00403715</initiator></group><session id=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\" group=\"urn:uuid:72561964-fe38-4eb5-be01-903ca224ca06\"><start-time>2016-11-30T16:52:20+00:00</start-time></session><participant id=\"urn:uuid:51504ec2-a6a7-40fa-8218-da33476f74de\" session=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\"><aor>sip:sub_00403715</aor><send><id>urn:uuid:74ca548a-0d5a-44b1-b95d-dfa169d497cc</id></send></participant><participant id=\"urn:uuid:ce173ed3-f8ed-41fc-9497-76d1c5b2d5f4\" session=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\"><aor>sip:238@10.255.2.40</aor><send><id>urn:uuid:a96d28a7-cf28-48c9-99e5-82d29d97b289</id></send></participant><stream id=\"urn:uuid:74ca548a-0d5a-44b1-b95d-dfa169d497cc\" session=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\"><label>1</label><mode>separate</mode></stream><stream id=\"urn:uuid:a96d28a7-cf28-48c9-99e5-82d29d97b289\" session=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\"><label>2</label><mode>separate</mode></stream><extensiondata id=\"urn:uuid:a469012b-0397-4813-aa37-67c48f1e6346\" parent=\"urn:uuid:387c9212-bc78-45cb-b1a1-bb6c5030c38b\"><broadWorksRecordingMetadata xmlns=\"http://schema.broadsoft.com/broadworksCallRecording\" version=\"2.0\"><extTrackingID>264:1</extTrackingID><serviceProviderID>customer_41142</serviceProviderID><groupID>customer_41142</groupID><userID>sub_00403715@lab.bbtnet.co.uk</userID><callID>callhalf-7342965:0</callID><callType><termCall><callingPartyNumber>sip:238@10.255.2.40</callingPartyNumber><calledPartyNumber>sip:+441133023237@10.255.2.40</calledPartyNumber></termCall></callType><recordingType>on</recordingType></broadWorksRecordingMetadata></extensiondata></recording_metadata>\0";

    fprintf(stderr, "%s\n", buf);

    XmlDocument doc = XmlDocument(buf, strlen(buf));

    return false;

}

bool testSimpleWithHeader() {
    const char * buf =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root p1=\"v1\"><n1>n1v1</n1></root>\0";

    fprintf(stderr, "%s\n", buf);

    XmlDocument doc = XmlDocument(buf, strlen(buf));

    return false;
}

bool testSimple1() {
    const char * buf =
        "<root p1=\"v1\" p2=\"v2\"><n1>subnode</n1></root>\0";

    fprintf(stderr, "%s\n", buf);

    XmlDocument doc = XmlDocument(buf, strlen(buf));

    return false;
}


bool testBasic1() {
    const char * buf =
            "<root property1=value1 property2=value2>value</root>";


    XmlDocument doc = XmlDocument(buf, strlen(buf));

    return false;
}

bool testSiprecInviteXml() {
    XmlDocument doc = XmlDocument(SIPRECINVITE, strlen(SIPRECINVITE));

    fprintf(stderr, "testSiprecInviteXml: loaded valid=%s\n", doc.isValid() ? "true":"false");
    std::string path;
    std::string value;

    std::string name;

    //path = std::string("recording_metadata.datamode");
    //path = std::string("recording_metadata.recording.id");
    //path = std::string("recording_metadata.invalid.id");
    //path = std::string("recording_metadata.xmlns");
    path = std::string("recording_metadata.extensiondata.broadWorksRecordingMetadata.callType.origCall.callingPartyNumber");

    value = doc.getValue(path, "NOT_FOUND");
    if (value.compare("complete")) {
        fprintf(stderr, "Read Value of %s returned '%s'\n", path.c_str(), value.c_str());
    }

//    path = std::string("recording_metadata.extensiondata.broadWorksRecordingMetadata.callType.origCall");
    path = std::string("recording_metadata.extensiondata.broadWorksRecordingMetadata");

    auto values = doc.getProperties(path);

    fprintf(stderr, "Read Properties of %s returned:\n", path.c_str());
    for (auto it = values.begin(); it != values.end(); ++it) {
        fprintf(stderr, "  Property '%s'='%s'\n", it->first.c_str(), it->second.c_str());
    }

    return false;
}

bool testGetValue() {
    return false;
}

int main(int argc, char * argv[]) {
    int failCount = 0;

    //failCount += (testSimple1() == false);
    failCount = (testSiprecInviteXml() == false);
    //failCount += (testBasic1() == false);
    //failCount += (test1() == false);

}
