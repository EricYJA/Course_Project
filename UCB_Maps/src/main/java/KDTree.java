import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.ArrayList;
import java.util.Comparator;



public class KDTree {
    private Map<Long, GraphDB.Node> refMap = new HashMap<>();
    private PriorityQueue<KDNode> closest;

    private class KDNode {
        long item;
        double x;
        double y;
        KDNode left;
        KDNode right;
        boolean isLat;


        KDNode(Long item, double x, double y,  boolean isLat) {
            this.item = item;
            left = null;
            right = null;
            this.x = x;
            this.y = y;
            this.isLat = isLat;
        }
    }

    private KDNode root;

    KDTree(Map<Long, GraphDB.Node> m) {
        boolean isLat = false;
        for (long l : m.keySet()) {
            refMap.put(l, new GraphDB.Node(l,
                    GraphDB.projectToX(m.get(l).getLon(), m.get(l).getLat()),
                    GraphDB.projectToY(m.get(l).getLon(), m.get(l).getLat())));
        }

//        System.out.println(refMap.toString());

        List<Long> axises = new ArrayList<>(m.keySet());
        axises.sort(Comparator.comparingDouble((o) -> refMap.get(o).getLon()));
        long mid = axises.get((axises.size() / 2));

        List<Long> starting = axises.subList(0, (axises.size() / 2));
        List<Long> endding = axises.subList((axises.size() / 2) + 1, axises.size());


        root = new KDNode(mid, refMap.get(mid).getLon(), refMap.get(mid).getLat(), isLat);
        root.left = buildingHelper(starting, true);
        root.right = buildingHelper(endding, true);


    }

    private KDNode buildingHelper(List<Long> items, boolean isLat) {
        if (items.size() < 1) {
            return null;
        } else if (items.size() == 1) {
            return new KDNode(items.get(0),
                    refMap.get(items.get(0)).getLon(), refMap.get(items.get(0)).getLat(), isLat);
        }

        if (isLat) {
            items.sort(Comparator.comparingDouble((o) -> refMap.get(o).getLat()));
        } else {
            items.sort(Comparator.comparingDouble((o) -> refMap.get(o).getLon()));
        }

        long mid = items.get(items.size() / 2);

        List<Long> starting = items.subList(0, (items.size() / 2));
        List<Long> endding = items.subList((items.size() / 2) + 1, items.size());

        KDNode newNode = new KDNode(mid, refMap.get(mid).getLon(), refMap.get(mid).getLat(), isLat);
        newNode.left = buildingHelper(starting, !isLat);
        newNode.right = buildingHelper(endding, !isLat);

        return newNode;
    }

    private void order(KDNode start) {
        if (start == null) {
            return;
        }
        System.out.println(start.item);
        order(start.left);
        order(start.right);
    }

    public void printOrder() {
        order(root);
    }

    public long nearestNeighbour(double lonS, double latS) {
        double lon = GraphDB.projectToX(lonS, latS);
        double lat = GraphDB.projectToY(lonS, latS);
        closest = new PriorityQueue<KDNode>(
                Comparator.comparingDouble((o) -> euclidean(o.x, lon, o.y, lat)));

        helper(root, lon, lat);

//        System.out.println(closest);
//
//        while (!closest.isEmpty()) {
//            System.out.println(closest.peek()
//                  +" - "+ euclidean(refMap.get(closest.peek()).getLon(), lon,
//                    refMap.get(closest.poll()).getLat(), lat));
//        }

        return closest.peek().item;
    }

    private void helper(KDNode currentNode, double lon, double lat) {
        /**
         * set the recursive base case, return when no children
         */
        if (currentNode == null) {
            return;
        }
        /**
         * defining the target position and make the search dependending on it
         */
        double refLength;
        closest.add(currentNode);

        if (currentNode.isLat) {
            /**
             * recursive to one end of the tree
             */

            if (lat < currentNode.y) {
                helper(currentNode.left, lon, lat);
            } else {
                helper(currentNode.right, lon, lat);
            }

            /**
             * Finding the distance from the target point to the current
             * spitting line and compare it to the closest distance so far
             */

            refLength = Math.abs(lat - currentNode.y);

            /**
             * recursive back to find it we need to explore another path
             */

            if (refLength < euclidean(closest.peek().x, lon,
                                        closest.peek().y, lat)) {
                if (lat < currentNode.y) {
                    helper(currentNode.right, lon, lat);
                } else {
                    helper(currentNode.left, lon, lat);
                }
            }

        } else {
            if (lon < currentNode.x) {
                helper(currentNode.left, lon, lat);
            } else {
                helper(currentNode.right, lon, lat);
            }

            refLength = Math.abs(lon - currentNode.x);

            if (refLength < euclidean(closest.peek().x, lon,
                                        closest.peek().y, lat)) {
                if (lon < currentNode.x) {
                    helper(currentNode.right, lon, lat);
                } else {
                    helper(currentNode.left, lon, lat);
                }
            }
        }

    }

    private double euclidean(double x1, double x2, double y1, double y2) {

//        double phi1 = Math.toRadians(y1);
//        double phi2 = Math.toRadians(y2);
//        double dphi = Math.toRadians(y1 - y2);
//        double dlambda = Math.toRadians(x1 - x2);
//
//        double a = Math.sin(dphi / 2.0) * Math.sin(dphi / 2.0);
//        a += Math.cos(phi1) * Math.cos(phi2) * Math.sin(dlambda / 2.0) * Math.sin(dlambda / 2.0);
//        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
//        return 3963 * c;

        return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
    }

}
