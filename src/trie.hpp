#ifndef TRIE_HPP
#define TRIE_HPP

#include <queue>
#include <vector>

class Trie {
private:
    std::vector<int> data; // 해당 지점에 데이터가 있다면 크기가 1이상, 없다면 0
    Trie *next[ 2 ];       // false, true
public:
    // 생성자
    Trie() : data( 0 ) {
        next[ 0 ] = nullptr;
        next[ 1 ] = nullptr;
    }

    // 소멸자
    ~Trie() {
        if ( next[ 0 ] )
            delete next[ 0 ];
        if ( next[ 1 ] )
            delete next[ 1 ];
    }

    // 트라이에 bool feature vector 삽입
    void insert( std::vector<bool> &feature, int label, size_t offset = 0 ) {
        if ( feature.size() == offset ) {
            data.push_back( label );
        } else {
            size_t curr = static_cast<size_t>( feature[ offset ] );
            if ( next[ curr ] == nullptr )
                next[ curr ] = new Trie();                      // 탐색이 처음되는 지점일 경우 동적할당
            next[ curr ]->insert( feature, label, offset + 2 ); // offset 한칸 오른쪽으로 옮겨서 추가
        }
    }

    // 트라이에서 문자열 찾기
    std::vector<int> find( std::vector<bool> &feature ) {
        std::queue<Trie *> q1; // queue of Trie*
        std::queue<int> q2;    // queue of offset

        q1.push( this );
        q2.push( 0 );

        std::vector<int> possible_answers;
        possible_answers.reserve( 100 );

        while ( !q1.empty() ) {
            auto curr = q1.front();
            auto offset = q2.front();
            q1.pop();
            q2.pop();

            if ( feature.size() == offset ) {
                if ( curr->data.size() != 0 ) {
                    possible_answers.insert( possible_answers.end(), curr->data.begin(), curr->data.end() );
                }
                continue;
            }

            if ( feature[ offset + 1 ] ) { // feature값이 0.5 +- margin범위라서 둘다 search함
                if ( curr->next[ 0 ] ) {
                    q1.push( curr->next[ 0 ] );
                    q2.push( offset + 2 );
                }
                if ( curr->next[ 1 ] ) {
                    q1.push( curr->next[ 1 ] );
                    q2.push( offset + 2 );
                }
                // 왼쪽, 오른쪽 subtree에서 모두 발견하지 못한 경우는 그냥 넘어감
                continue;
            } else {
                size_t idx = static_cast<size_t>( feature[ offset ] ); // 0 or 1
                if ( curr->next[ idx ] == nullptr )
                    continue; // 찾는 값이 존재하지 않음
                else {
                    q1.push( curr->next[ idx ] );
                    q2.push( offset + 2 );
                }
            }
        }
        return possible_answers;
    }
};

#endif // TRIE_HPP