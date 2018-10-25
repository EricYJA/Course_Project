import java.util.PriorityQueue;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.Collections;
import java.util.Objects;
import java.util.Comparator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * This class provides a <code>shortestPath</code> method and <code>routeDirections</code> for
 * finding routes between two points on the map.
 */
public class Router {
    /**
     * Return a <code>List</code> of vertex IDs corresponding to the shortest path from a given
     * starting coordinate and destination coordinate.
     * @param g <code>GraphDB</code> data source.
     * @param stlon The longitude of the starting coordinate.
     * @param stlat The latitude of the starting coordinate.
     * @param destlon The longitude of the destination coordinate.
     * @param destlat The latitude of the destination coordinate.
     * @return The <code>List</code> of vertex IDs corresponding to the shortest path.
     */
    public static List<Long> shortestPath(GraphDB g,
                                          double stlon, double stlat,
                                          double destlon, double destlat) {
        // TODO
        LinkedList<Long> path = new LinkedList<>();

        Set<Long> mark = new HashSet<>();

        /** get ID of start and destination node
         *
         */
        Long startNode = g.closest(stlon, stlat);
        Long destNode = g.closest(destlon, destlat);

        PriorityQueue<Entry> fringe = new PriorityQueue<>(Comparator.comparing(Entry::getPriority));

        Best best = new Best(g, startNode);
        /** Start by adding the all vertices to the fringe,
         * such that all vertices have infinite priorities, except the source with priority 0.
         */

//        for (long num : g.vertices()) {
//            fringe.add(new Entry(num, Integer.MAX_VALUE));
//        }

        fringe.add(new Entry(startNode, 0));
        long v;

        while (true) {
            v = fringe.poll().getId();

            if (mark.contains(v)) {
                continue;
            }

            if (v == destNode) {
                break;
            }

            mark.add(v);

//            for (long w : g.adjacent(v)) {
//                if (v != startNode && w == best.getNodeBestNode(v)) {
//                    continue;
//                }
//                best.setBestDis(w, best.nodeBestDis.get(v) + g.distance(v, w));
//            }



            relax(g, fringe, best, v, destNode, mark, startNode);
        }
        /** 从终点开始，把best parent node按顺序放进path
         *
         */
        Long id = destNode;
        path.addFirst(id);
        while (!id.equals(startNode)) {
            id = best.getNodeBestNode(id);
            path.addFirst(id);
        }
        return path;
    }


    static void relax(GraphDB g, PriorityQueue<Entry> fringe,
                                       Best best, Long v, Long destNode, Set mark, long startNode) {

        for (long w : g.adjacent(v)) {
            if (best.getNodeBestDis(v) + g.distance(v, w) < best.getNodeBestDis(w)) {

                best.setBestDis(w, best.getNodeBestDis(v)
                        + g.distance(v, w)); //update best so that d(s, w) = d(s, v) + ed(v, w)
                best.setBestNode(w, v);

                fringe.add(new Entry(w,
                        best.getNodeBestDis(v)
                                + g.distance(v, w)
                                + g.distance(w, destNode)
                ));
            }
        }
    }

    static class Entry {

        private long id;
        private double priority;

        Entry(long id, double priority) {
            this.id = id;
            this.priority = priority;
        }

        public double getPriority() {
            return priority;
        }

        public long getId() {
            return id;
        }
    }

    static class Best {

        //map from Node ID to best known distance
        private HashMap<Long, Double> nodeBestDis = new HashMap<>();
        //map from Node ID to best Node ID
        private HashMap<Long, Long> nodeBestNode = new HashMap<>();

        Best(GraphDB g, long s) {
            for (Long nodeID : g.vertices()) {
                setBestDis(nodeID, Double.POSITIVE_INFINITY);
            }
            setBestDis(s, 0);
        }

        public void setBestDis(long id, double dis) {
            this.nodeBestDis.put(id, dis);
        }

        public void setBestNode(long vertex, long parent) {
            this.nodeBestNode.put(vertex, parent);
        }

        public double getNodeBestDis(Long id) {
            return nodeBestDis.get(id);
        }

        public long getNodeBestNode(long id) {
            return nodeBestNode.get(id);
        }
    }
    /**
     * Given a <code>route</code> of vertex IDs, return a <code>List</code> of
     * <code>NavigationDirection</code> objects representing the travel directions in order.
     * @param g <code>GraphDB</code> data source.
     * @param route The shortest-path route of vertex IDs.
     * @return A new <code>List</code> of <code>NavigationDirection</code> objects.
     */
    public static List<NavigationDirection> routeDirections(GraphDB g, List<Long> route) {
        // TODO
        return Collections.emptyList();
    }

    /**
     * Class to represent a navigation direction, which consists of 3 attributes:
     * a direction to go, a way, and the distance to travel for.
     */
    public static class NavigationDirection {

        /** Integer constants representing directions. */
        public static final int START = 0, STRAIGHT = 1, SLIGHT_LEFT = 2, SLIGHT_RIGHT = 3,
                RIGHT = 4, LEFT = 5, SHARP_LEFT = 6, SHARP_RIGHT = 7;

        /** Number of directions supported. */
        public static final int NUM_DIRECTIONS = 8;

        /** A mapping of integer values to directions.*/
        public static final String[] DIRECTIONS = new String[NUM_DIRECTIONS];

        static {
            DIRECTIONS[START] = "Start";
            DIRECTIONS[STRAIGHT] = "Go straight";
            DIRECTIONS[SLIGHT_LEFT] = "Slight left";
            DIRECTIONS[SLIGHT_RIGHT] = "Slight right";
            DIRECTIONS[RIGHT] = "Turn right";
            DIRECTIONS[LEFT] = "Turn left";
            DIRECTIONS[SHARP_LEFT] = "Sharp left";
            DIRECTIONS[SHARP_RIGHT] = "Sharp right";
        }

        /** The direction represented.*/
        int direction;
        /** The name of this way. */
        String way;
        /** The distance along this way. */
        double distance = 0.0;

        public String toString() {
            return String.format("%s on %s and continue for %.3f miles.",
                    DIRECTIONS[direction], way, distance);
        }

        /**
         * Returns a new <code>NavigationDirection</code> from a string representation.
         * @param dirAsString <code>String</code> instructions for a navigation direction.
         * @return A new <code>NavigationDirection</code> based on the string, or <code>null</code>
         * if unable to parse.
         */
        public static NavigationDirection fromString(String dirAsString) {
            String regex = "([a-zA-Z\\s]+) on ([\\w\\s]*) and continue for ([0-9\\.]+) miles\\.";
            Pattern p = Pattern.compile(regex);
            Matcher m = p.matcher(dirAsString);
            NavigationDirection nd = new NavigationDirection();
            if (m.matches()) {
                String direction = m.group(1);
                if (direction.equals("Start")) {
                    nd.direction = NavigationDirection.START;
                } else if (direction.equals("Go straight")) {
                    nd.direction = NavigationDirection.STRAIGHT;
                } else if (direction.equals("Slight left")) {
                    nd.direction = NavigationDirection.SLIGHT_LEFT;
                } else if (direction.equals("Slight right")) {
                    nd.direction = NavigationDirection.SLIGHT_RIGHT;
                } else if (direction.equals("Turn right")) {
                    nd.direction = NavigationDirection.RIGHT;
                } else if (direction.equals("Turn left")) {
                    nd.direction = NavigationDirection.LEFT;
                } else if (direction.equals("Sharp left")) {
                    nd.direction = NavigationDirection.SHARP_LEFT;
                } else if (direction.equals("Sharp right")) {
                    nd.direction = NavigationDirection.SHARP_RIGHT;
                } else {
                    return null;
                }

                nd.way = m.group(2);
                try {
                    nd.distance = Double.parseDouble(m.group(3));
                } catch (NumberFormatException e) {
                    return null;
                }
                return nd;
            } else {
                // Not a valid nd
                return null;
            }
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof NavigationDirection) {
                return direction == ((NavigationDirection) o).direction
                        && way.equals(((NavigationDirection) o).way)
                        && distance == ((NavigationDirection) o).distance;
            }
            return false;
        }

        @Override
        public int hashCode() {
            return Objects.hash(direction, way, distance);
        }
    }
}
