class Server {
    static function main() {
        var s = new neko.net.Socket();
        s.bind(new neko.net.Host("localhost"),843);
        s.listen(1);
        trace("Starting server...");
        while( true ) {
            var c : neko.net.Socket = s.accept();
            trace("Client connected...");
            c.write('<?xml version="1.0" encoding="UTF-8"?><cross-domain-policy xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://www.adobe.com/xml/schemas/PolicyFileSocket.xsd"><allow-access-from domain="*" to-ports="*" secure="false" /><site-control permitted-cross-domain-policies="master-only" /></cross-domain-policy>');
            c.output.writeByte(0);
            c.write("your IP is "+c.peer().host.toString()+"\n");
            c.output.writeByte(0);
            c.write("exit");
            c.output.writeByte(0);
            c.close();
        }
    }
}
