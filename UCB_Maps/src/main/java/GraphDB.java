import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Collections;


/**
 * Graph for storing all of the intersection (vertex) and road (edge) information.
 * Uses your GraphBuildingHandler to convert the XML files into a graph. Your
 * code must include the vertices, adjacent, distance, closest, lat, and lon
 * methods. You'll also need to include instance variables and methods for
 * modifying the graph (e.g. addNode and addEdge).
 *
 * @author Kevin Lowe, Antares Chen, Kevin Lin
 */
public class GraphDB {

    /** Fields; maps
     *
     */
    private Map<Long, Node> nodes = new HashMap<>();

    private Map<Long, ArrayList<Edge>> edges = new HashMap<>();

    private Map<Long, ArrayList<Long>> adjacents = new HashMap<>();

    private KDTree closest;

    /**
     * This constructor creates and starts an XML parser, cleans the nodes, and prepares the
     * data structures for processing. Modify this constructor to initialize your data structures.
     * @param dbPath Path to the XML file to be parsed.
     */
    public GraphDB(String dbPath) {
        File inputFile = new File(dbPath);
        try (FileInputStream inputStream = new FileInputStream(inputFile)) {
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser saxParser = factory.newSAXParser();
            saxParser.parse(inputStream, new GraphBuildingHandler(this));
        } catch (ParserConfigurationException | SAXException | IOException e) {
            e.printStackTrace();
        }
        clean();
        closest = new KDTree(nodes);

        for (Node n : nodes.values()) {
            if (n.getName() != null) {
                System.out.println(true);
            }
        }
    }

    /** Classes Node & Edge */

    static class Node {
        private long id;
        private double lon;
        private double lat;
        private String name;

        Node(long id, double lon, double lat) {
            this.id = id;
            this.lon = lon;
            this.lat = lat;
        }

        public void setName(String name) {
            this.name = name;
        }

        public boolean isLocation() {
            return name != null;
        }

        public String getName() {
            return name;
        }

        public long getId() {
            return id;
        }

        public double getLon() {
            return lon;
        }

        public double getLat() {
            return lat;
        }

        @Override
        public String toString() {
            return "Node{"
                    + "id=" + id
                    + ", lon=" + lon
                    + ", lat=" + lat
                    + ", name='" + name + '\'' + '}';
        }
    }

    static class Edge {
        private long from;
        private long to;
        private double weight;

        Edge(long from, long to, double weight) {
            this.from = from;
            this.to = to;
            this.weight = weight;
        }

        public long getFrom() {
            return from;
        }

        public long getTo() {
            return to;
        }

        public double getWeight() {
            return weight;
        }
    }

    /** Method addNode addEdge and removeNode */
    public void addNode(Node node) {
        nodes.put(node.getId(), node);

        edges.put(node.getId(), new ArrayList<>());

        adjacents.put(node.getId(), new ArrayList<>());
    }

    public void addNode(long id, double lon, double lat) {
        nodes.put(id, new Node(id, lon, lat));

        edges.put(id, new ArrayList<>());

        adjacents.put(id, new ArrayList<>());
    }

    public void addEdge(Long from, Long to) {
        edges.get(from).add(new Edge(from, to, distance(from, to)));
        edges.get(to).add(new Edge(to, from, distance(from, to)));

        adjacents.get(from).add(to);
        adjacents.get(to).add(from);
    }

    public void removeNode(long id) {
        nodes.remove(id);
        edges.remove(id);
        adjacents.remove(id);
    }
    /**
     * Helper to process strings into their "cleaned" form, ignoring punctuation and capitalization.
     * @param s Input string.
     * @return Cleaned string.
     */
    private static String cleanString(String s) {
        return s.replaceAll("[^a-zA-Z ]", "").toLowerCase();
    }

    /**
     * Remove nodes with no connections from the graph.
     * While this does not guarantee that any two nodes in the remaining graph are connected,
     * we can reasonably assume this since typically roads are connected.
     */
    private void clean() {
        List<Long> toRemove = new ArrayList<>();
        for (long id: nodes.keySet()) {
            if (edges.get(id).isEmpty()) {
                toRemove.add(id);
            }
        }

        for (long l : toRemove) {
            removeNode(l);
        }
    }

    /**
     * Returns the longitude of vertex <code>v</code>.
     * @param v The ID of a vertex in the graph.
     * @return The longitude of that vertex, or 0.0 if the vertex is not in the graph.
     */
    double lon(long v) {
        return nodes.get(v).getLon();
    }

    /**
     * Returns the latitude of vertex <code>v</code>.
     * @param v The ID of a vertex in the graph.
     * @return The latitude of that vertex, or 0.0 if the vertex is not in the graph.
     */
    double lat(long v) {
        return nodes.get(v).getLat();
    }

    /**
     * Returns an iterable of all vertex IDs in the graph.
     * @return An iterable of all vertex IDs in the graph.
     */
    Iterable<Long> vertices() {
        return nodes.keySet();
    }
    int size() {
        return nodes.size();
    }
    /**
     * Returns an iterable over the IDs of all vertices adjacent to <code>v</code>.
     * @param v The ID for any vertex in the graph.
     * @return An iterable over the IDs of all vertices adjacent to <code>v</code>, or an empty
     * iterable if the vertex is not in the graph.
     */
    Iterable<Long> adjacent(long v) {
        return adjacents.get(v);
    }

    /**
     * Returns the great-circle distance between two vertices, v and w, in miles.
     * Assumes the lon/lat methods are implemented properly.
     * @param v The ID for the first vertex.
     * @param w The ID for the second vertex.
     * @return The great-circle distance between vertices and w.
     * @source https://www.movable-type.co.uk/scripts/latlong.html
     */
    public double distance(long v, long w) {
        double phi1 = Math.toRadians(lat(v));
        double phi2 = Math.toRadians(lat(w));
        double dphi = Math.toRadians(lat(w) - lat(v));
        double dlambda = Math.toRadians(lon(w) - lon(v));

        double a = Math.sin(dphi / 2.0) * Math.sin(dphi / 2.0);
        a += Math.cos(phi1) * Math.cos(phi2) * Math.sin(dlambda / 2.0) * Math.sin(dlambda / 2.0);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        return R * c;
    }

    /**
     * Returns the ID of the vertex closest to the given longitude and latitude.
     * @param lon The given longitude.
     * @param lat The given latitude.
     * @return The ID for the vertex closest to the <code>lon</code> and <code>lat</code>.
     */
    public long closest(double lon, double lat) {
        long dst = closest.nearestNeighbour(lon, lat);

//        double dis = -1;
//        long dst = -1;
//        Node tmp = new Node(-1, lon, lat);
//        addNode(tmp);
//
//        for (long l : nodes.keySet()) {
//            if (l == -1) {
//                continue;
//            }
//
//            if (dis == -1) {
//                dis = distance(tmp.getId(), l);
//                dst = l;
//            } else {
//                dst = dis > distance(tmp.getId(), l)
//                        ? l : dst;
//                dis = dis > distance(tmp.getId(), l)
//                        ? distance(tmp.getId(), l) : dis;
//            }
//        }
//        removeNode(tmp.getId());
//
        return dst;
    }

    /**
     * Return the Euclidean x-value for some point, p, in Berkeley. Found by computing the
     * Transverse Mercator projection centered at Berkeley.
     * @param lon The longitude for p.
     * @param lat The latitude for p.
     * @return The flattened, Euclidean x-value for p.
     * @source https://en.wikipedia.org/wiki/Transverse_Mercator_projection
     */
    static double projectToX(double lon, double lat) {
        double dlon = Math.toRadians(lon - ROOT_LON);
        double phi = Math.toRadians(lat);
        double b = Math.sin(dlon) * Math.cos(phi);
        return (K0 / 2) * Math.log((1 + b) / (1 - b));
    }

    /**
     * Return the Euclidean y-value for some point, p, in Berkeley. Found by computing the
     * Transverse Mercator projection centered at Berkeley.
     * @param lon The longitude for p.
     * @param lat The latitude for p.
     * @return The flattened, Euclidean y-value for p.
     * @source https://en.wikipedia.org/wiki/Transverse_Mercator_projection
     */
    static double projectToY(double lon, double lat) {
        double dlon = Math.toRadians(lon - ROOT_LON);
        double phi = Math.toRadians(lat);
        double con = Math.atan(Math.tan(phi) / Math.cos(dlon));
        return K0 * (con - Math.toRadians(ROOT_LAT));
    }

    /**
     * In linear time, collect all the names of OSM locations that prefix-match the query string.
     * @param prefix Prefix string to be searched for. Could be any case, with our without
     *               punctuation.
     * @return A <code>List</code> of the full names of locations whose cleaned name matches the
     * cleaned <code>prefix</code>.
     */
    public List<String> getLocationsByPrefix(String prefix) {
        return Collections.emptyList();
    }

    /**
     * This will set the name of the location in order to set it as a location
     * @param node the id of node which need to be named
     * @param name set the node's name to name
     */

    public void setLocation(long node, String name) {
        Node location = nodes.get(node);
        location.setName(name);
    }

    /**
     * Collect all locations that match a cleaned <code>locationName</code>, and return
     * information about each node that matches.
     * @param locationName A full name of a location searched for.
     * @return A <code>List</code> of <code>LocationParams</code> whose cleaned name matches the
     * cleaned <code>locationName</code>
     */
    public List<LocationParams> getLocations(String locationName) {
        return Collections.emptyList();
    }

    /**
     * Returns the initial bearing between vertices <code>v</code> and <code>w</code> in degrees.
     * The initial bearing is the angle that, if followed in a straight line along a great-circle
     * arc from the starting point, would take you to the end point.
     * Assumes the lon/lat methods are implemented properly.
     * @param v The ID for the first vertex.
     * @param w The ID for the second vertex.
     * @return The bearing between <code>v</code> and <code>w</code> in degrees.
     * @source https://www.movable-type.co.uk/scripts/latlong.html
     */
    double bearing(long v, long w) {
        double phi1 = Math.toRadians(lat(v));
        double phi2 = Math.toRadians(lat(w));
        double lambda1 = Math.toRadians(lon(v));
        double lambda2 = Math.toRadians(lon(w));

        double y = Math.sin(lambda2 - lambda1) * Math.cos(phi2);
        double x = Math.cos(phi1) * Math.sin(phi2);
        x -= Math.sin(phi1) * Math.cos(phi2) * Math.cos(lambda2 - lambda1);
        return Math.toDegrees(Math.atan2(y, x));
    }



    /** Radius of the Earth in miles. */
    private static final int R = 3963;
    /** Latitude centered on Berkeley. */
    private static final double ROOT_LAT = (MapServer.ROOT_ULLAT + MapServer.ROOT_LRLAT) / 2;
    /** Longitude centered on Berkeley. */
    private static final double ROOT_LON = (MapServer.ROOT_ULLON + MapServer.ROOT_LRLON) / 2;
    /**
     * Scale factor at the natural origin, Berkeley. Prefer to use 1 instead of 0.9996 as in UTM.
     * @source https://gis.stackexchange.com/a/7298
     */
    private static final double K0 = 1.0;
}
