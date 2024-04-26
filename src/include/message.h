#ifndef CPM_MESSAGE
#define CPM_MESSAGE

#include "dialog.h"
#include "object.h"
#include <cstdint>
#include <string>
#include <set>
#include <unordered_map>

namespace cpm {

    namespace builder {

        class MessageContent : public AbstractMessageContent {
            MessageContent(const std::vector<MessageHeader>& headers);
        public:
            friend class MessageBuilder;
            const std::string& cc;
            const std::string& bcc;
            const std::string& pAssertedService;
            const std::string& acceptContact;
            const std::string& messageCorrelator;
            const std::string& messageContext;
        };

        class MessageBuilder {
            std::vector<MessageHeader> headers;

            struct MessageBodyBuilder : HeaderBuilder {
                MessageContent MessageBody(const std::string& messageBody);
            };
            struct ContentTypeBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Set to the following value: “Message/CPIM” for objects including a CPM Standalone Message, a
                 * legacy message or an IMDN message.
                 *
                 * Other Content-Types than “Message/CPIM” MAY be stored inside the “Message/CPIM” wrapper, some
                 * examples are listed above in this section.
                 */
                MessageBodyBuilder ContentType(const std::string& contentType);
            };
            struct MessageContextBuilder : ContentTypeBuilder {
                /**
                 * [Optional]
                 * This MIME header field is defined in [RFC3458].
                 *
                 * It is only present when a CPM Message was sent or delivered via legacy messaging
                 * such as SMS or MMS.
                 *
                 * It SHALL be present if a Message-Correlator header is present.
                 *
                 * In this case, this MIME header is set to the value “pager- message” for SMS and to
                 * “multimedia-message” for MMS.
                 *
                 * For received messages, the value is set when the message is delivered to a primary device:
                 * - via SMS , the value is “pager-message”;
                 * - via MMS, the value is set to “multimedia-message”.
                 *
                 * For originated messages, when the legacy message was sent:
                 * - via SMS, the value is set to “pager-message”;
                 * - via MMS, the value is set to “multimedia-message”.
                 */
                ContentTypeBuilder MessageContext(const std::string& messageContext);
            };
            struct MessageCorrelatorBuilder : MessageContextBuilder {
                /**
                 * [Optional]
                 * For correlation of legacy messages such as SMS and MMS.
                 *
                 * For SMS message objects the value SHALL contain a fragment or the full text
                 * contained in the user data of the short message, depending on the length of the
                 * message.
                 * For MMS message objects the value SHALL be set to the Message-ID field value as
                 * defined in [OMA-MMS-ENC].
                 *
                 * The type of value of the Message-Correlator SHALL correspond to the context
                 * indicated in the value of the Message- Context header.
                 *
                 * Note: this is a new field, extending the Internet Message Format in accordance
                 * with 3.6.8 of [RFC5322].
                 */
                MessageContextBuilder MessageCorrelator(const std::string& messageCorrelator);
            };
            struct IMDNMessageIDBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * For messages received via MSRP, it is set to the imdn.Message- ID header field
                 * value described in sect. 6.3 of [RFC5438] when available in the CPM request
                 * (e.g., within a CPM Standalone Large Message Mode, or CPM Session, or CPM File
                 * Transfer).
                 *
                 * For SMS Messages, MMS Messages, CPM Pager Mode Messages without an imdn.
                 * Message-ID, session info objects, Group State Objects and Conference Info
                 * Objects a unique value needs to be generated and assigned by the storing entity
                 * (e.g. CPM Participating Function, CPM Message Store Client), with similar
                 * definition and uniqueness requirements as described in sect. 6.3 of [RFC5438]
                 *
                 * Note: this is a new field, extending the Internet Message Format in accordance
                 * with 3.6.8 of [RFC5322].
                 */
                MessageCorrelatorBuilder IMDNMessageID(const std::string& imdnMessageId);
            };
            struct AcceptContactBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Occurrence: 1..*
                 * Each occurrence of this MIME header SHALL be set to the value of an
                 * Accept-Contact header field that was present in the SIP request associated to
                 * this object
                 */
                IMDNMessageIDBuilder AcceptContact(const std::string& acceptContact);
            };
            struct InReplyToContributionIDBuilder : AcceptContactBuilder {
                /**
                 * [Optional]
                 * Set to the InReplyTo-Contribution-ID of the SIP MESSAGE or SIP INVITE request.
                 *
                 * Note: this is a new field, extending the Internet Message Format in accordance
                 * with 3.6.8 of [RFC5322].
                 *
                 * Note: it is not present in CPM Chat Messages (included in the session info
                 * object instead).
                 */
                AcceptContactBuilder InReplyToContributionID(const std::string& inReplyToContributionId);
            };
            struct ContributionIDBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Set to the Contribution-ID of the SIP MESSAGE or SIP INVITE request.
                 *
                 * Note: this is a new field, extending the Internet Message Format in accordance
                 * with 3.6.8 of [RFC5322].
                 */
                InReplyToContributionIDBuilder ContributionID(const std::string& contributionId);
            };
            struct ConversationIDBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Set to the Conversation-ID of the SIP MESSAGE or SIP INVITE request.
                 *
                 * Note: this is a new field, extending the Internet Message Format in accordance
                 * with 3.6.8 of [RFC5322].
                 */
                ContributionIDBuilder ConversationID(const std::string& conversationId);
            };
            struct PAssertedServiceBuilder : ConversationIDBuilder {
                /**
                 * [Optional]
                 * Includes the CPM Feature Tag present in the SIP request. It SHALL be present in
                 * message objects containing:
                 * - CPM Standalone Messages,
                 * - IMDN notifications received via SIP MESSAGE
                 * - File Transfer History objects.
                 *
                 * Note: it is not present in:
                 * - CPM Chat Messages (as it is included in the session infoobject instead)
                 * - Group State Object (as it is included in the session info object instead)
                 * - Conference Information Object (as it is included in the session info object
                 * instead)
                 * - legacy messages (containing SMS or MMS).
                 */
                ConversationIDBuilder PAssertedService(const std::string& pAssertedService);
            };
            struct SubjectBuilder : PAssertedServiceBuilder {
                /**
                 * [Optional]
                 * Set only if “Subject” header is set in the SIP request.
                 *
                 * Note: it is not present in CPM Chat Messages (included in the session info
                 * object instead).
                 */
                PAssertedServiceBuilder Subject(const std::string& subject);
            };
            struct DateBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Set to, in order of preference:
                 * - the Date of the SIP MESSAGE or SIP INVITE request if available, otherwise,
                 * - the date and time when the recording of the CPM request was started.
                 */
                SubjectBuilder Date(const std::string& date);
            };
            struct BCCBuilder : DateBuilder {
                /**
                 * [Optional]
                 * Used only in MMS objects.
                 */
                DateBuilder BCC(const std::string& bcc);
            };
            struct CCBuilder : BCCBuilder {
                /**
                 * [Optional]
                 * Used only in MMS objects.
                 */
                BCCBuilder CC(const std::string& cc);
            };
            struct ToBuilder : HeaderBuilder {
                /**
                 * [Mandatory]
                 * Set to the address of the recipient of the CPM request or response, or of the
                 * legacy message.
                 *
                 * For the CPM request or response, it is retrieved from the authenticated
                 * recipient’s CPM Address in the SIP request or SIP 200 “OK” response to the SIP
                 * request, with the following clarifications:
                 * - For CPM Chat Messages exchanged within a CPM Group Session, it is set to the
                 * address of the recipient of the CPM Chat Message, as retrieved from the CPIM To
                 * header of the MSRP message.
                 * * In a CPM Chat Message sent by a Participant to the other CPM Group Session
                 * Participants, this is the address of the CPM Group Session Identity;
                 * * in a CPM Chat Message received by a Participant in the CPM Group Session, it
                 * is the address of the Participant.
                 *
                 * For a legacy message (e.g. SMS, MMS) it is set to the authenticated address of
                 * the recipient:
                 * - To comply with the MIME To header syntax, a domain part needs to be added to
                 * the telephone number. The appended domain part SHALL be a configurable String
                 * defined by the service provider, allowing the CPM Message Store Client to
                 * determine that the URI is not a real SIP URI, but is rather based on a telephone
                 * number only. (Example: +15145551234\@phony.com)
                 */
                CCBuilder To(const std::string& to);
            };

        public:
            /**
             * [Mandatory]
             * Set to the address of the initiator of the CPM request or response, or legacy
             * message.
             *
             * For the CPM request or response, it is retrieved from the authenticated
             * originator’s CPM Address in the SIP request, as follows:
             * - Set to the value of the Referred-By header field if it is present in the SIP
             * request; or
             * - Set to the value of the P-Asserted-Identity received in the SIP request or
             * response.
             * -For CPM Chat Messages exchanged in a 1-1 CPM Session it is set to the
             * authenticated CPM Address of the CPM Chat Messages originator, as retrieved
             * from the SIP request or response;
             * - For CPM Chat Messages exchanged within a CPM Group Session, it is set to the
             * address of the originator of the message as retrieved from the CPIM From header.
             *
             * For a legacy message (e.g. SMS, MMS) it is set to the authenticated address of
             * the originating party:
             * - To comply with the MIME From header syntax, a domain part needs to be added
             * to the telephone number. The appended domain part SHALL be a configurable String
             * defined by the service provider, allowing the CPM Message Store Client to
             * determine that the URI is not a real SIP URI, but is rather based on a telephone
             * number only. (Example: +15145551234\@phony.com)
             *
             * If the initiator withholds its details, (e.g. use of anonymous URI) then the
             * “anonymous” String SHALL be provided.
             */
            ToBuilder From(const std::string& from);
        };

    } // namespace builder

    class Message : AbstractMessageObject<builder::MessageContent> {
    public:
        Message(const Identifier& identifier, const builder::MessageContent& content);
        using Content = builder::MessageContent;
        using Builder = builder::MessageBuilder;
    };

}

#endif // CPM_MESSAGE